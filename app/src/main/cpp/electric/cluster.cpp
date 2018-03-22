#include "cluster.h"
#include "../engine/matrix.h"
#include "vertex.h"
#include "engine.h"

namespace Electric {

	void Cluster::init(std::mt19937& generator, Engine::BinaryReader& reader, DischargeListener* pDischargeListener)
	{
		m_generator = generator;
		read(reader, pDischargeListener);
	}

	void Cluster::read(Engine::BinaryReader& reader, DischargeListener* pDischargeListener)
	{
		m_states.clear();

		m_dirty = true;
		m_position.read(reader);
		m_rotation.read(reader);

		m_switchInterval = reader.read<float>();
		m_transitionTime = reader.read<float>();

		read(reader, m_chargePath);
		int nrNodes = reader.read<int>();
		int diff = nrNodes - m_nrNodes;

		if (diff < 0)
		{
			m_nodes.resize(static_cast<UINT>(nrNodes));
		}
		else
		{
			for(int i=0; i<diff; ++i)
			{
				m_nodes.push_back(createNode(pDischargeListener));
			}
		}

		m_nrNodes = nrNodes;

		int nrStates = reader.read<int>();
		for(int i=0; i<nrStates; ++i)
		{
			State state;
			read(reader, state);
			m_states.push_back(state);
		}

		toState(m_states[0], 1.0f);
	}

	auto Cluster::createNode(DischargeListener* pDischargeListener)->node_ptr
	{
		std::uniform_real_distribution<> dist(0.0f, 1.0f);
		float dt = static_cast<float>(dist(m_generator));
		float charge = m_chargePath.traverse(static_cast<float>(dist(m_generator)));

		node_ptr pNode(new Node( {0,0,0}, {0,0,0}, 1.0f, charge, dt, pDischargeListener));
		m_nodes.push_back(pNode);
		return pNode;
	}

	void Cluster::setState(const State& state)
	{
		m_states.clear();
		m_states.push_back(state);
		m_state = 0;
		toState(state, 0.1f);
	}

	void Cluster::read(Engine::BinaryReader& reader, vec3_path& path, const Math::Vector3 def)
	{
		int nrParts = reader.read<int>();
		if(nrParts <= 0)
		{
			path.add(1.0f, 1, &def);
			return;
		}

		for(int i=0; i<nrParts; ++i)
		{
			float time = reader.read<float>();
			int nrPoints = reader.read<int>();
			std::unique_ptr<Math::Vector3[]> p(new Math::Vector3[nrPoints]);
			for(int j=0; j<nrPoints; ++j)
				p[j].read(reader);
			path.add(time, static_cast<UINT>(nrPoints), p.get());
		}
	}

	void Cluster::read(Engine::BinaryReader& reader, float_path& path, const float def)
	{
		int nrParts = reader.read<int>();
		if(nrParts <= 0)
		{
			path.add(1.0f, 1, &def);
			return;
		}

		for(int i=0; i<nrParts; ++i)
		{
			float time = reader.read<float>();
			int nrPoints = reader.read<int>();
			std::unique_ptr<float[]> p(new float[nrPoints]);
			for(int j=0; j<nrPoints; ++j)
				p[j] = reader.read<float>();
			path.add(time, static_cast<UINT>(nrPoints), p.get());
		}
	}

	void Cluster::read(Engine::BinaryReader& reader, State& state)
	{
		read(reader, state.positionPath);
		read(reader, state.rotationPath);
		read(reader, state.offsetPath);
		read(reader, state.spreadPath);
		read(reader, state.spreadDirectionYawPath);
		read(reader, state.spreadDirectionPatchPath);
		read(reader, state.colorPath, {1,1,1});
		read(reader, state.sizePath, 1);
	}

	void Cluster::toState(const State& state, float transitionTime)
	{
		std::uniform_real_distribution<> dist(0.0f, 1.0f);
		for(auto& it : m_nodes)
		{
			float d = it->getDelta();
			Math::Matrix rot;
			{
				Math::Vector3 r = state.rotationPath.traverse(d);
				Math::Matrix::setRotate(rot, r.x(), r.y(), r.z());
			}

			float yaw = state.spreadDirectionYawPath.traverse(d);
			std::uniform_real_distribution<> yawDist(-yaw, yaw);
			yaw = static_cast<float>(yawDist(m_generator));

			float pitch = state.spreadDirectionPatchPath.traverse(d);
			std::uniform_real_distribution<> pitchDist(-pitch, pitch);
			pitch = static_cast<float>(pitchDist(m_generator));

			float a = static_cast<float>(dist(m_generator)) * 2.0f * static_cast<float>(M_PI);
			float r = static_cast<float>(sqrt(dist(m_generator)));

			yaw = r * static_cast<float>(cos(a)) * yaw;
			pitch = r * static_cast<float>(sin(a)) * pitch;

			Math::Matrix rotSpread;
			Math::Matrix::setRotate(rotSpread, yaw, pitch, 0);
			rotSpread = rotSpread * rot;

			Math::Vector3 p = rotSpread.transform({0,0,-1});

			float offset = state.offsetPath.traverse(d);
			float spread = state.spreadPath.traverse(d);
			std::uniform_real_distribution<> offsetDist(-spread, spread);

			p *= (offset + static_cast<float>(offsetDist(m_generator)));
			p += state.positionPath.traverse(d);

			float s = state.sizePath.traverse(d);

			Math::Vector3 color = state.colorPath.traverse(d);

			std::shared_ptr<vec3_path> offsetPath(new vec3_path());
			{
				Math::Vector3 start = it->getOffset();
				Math::Vector3 diff = p - start;
				Math::Vector3 points[] = {start,
									  start + (diff * 0.95f),
									  start + (diff * 0.99f),
									  p
				};
				offsetPath->add(transitionTime, 4, points);
			};

			std::shared_ptr<vec3_path> colorPath(new vec3_path());
			{
				Math::Vector3 points[] = {it->getColor(), color};
				colorPath->add(transitionTime, 2, points);
			};

			std::shared_ptr<float_path> scalePath(new float_path());
			{
				float points[] = {it->getScale(), s };
				scalePath->add(transitionTime, 2, points);
			};

			it->transition(offsetPath, colorPath, scalePath);
		}
	}

	void Cluster::update(const Math::Matrix& parent, bool dirty, std::vector<Search>& searches, float dt)
	{
		m_time -= dt;
		if(m_time <= 0.0f)
		{
			m_state = ++m_state % static_cast<UINT>(m_states.size());
			m_time = m_switchInterval;
			toState(m_states[m_state], 5.0f);
		}

		dirty |= m_dirty;
		if(dirty)
		{
			Math::Matrix::setRotate(m_transform, m_rotation.x(), m_rotation.y(), m_rotation.z());
			Math::Matrix::translate(m_transform,
									m_position.x(),
									m_position.y(),
									m_position.z());
			m_transform = Math::Matrix::multiply(m_transform, parent);
			m_dirty = false;
		}

		for(auto& it : m_nodes)
		{
			it->update(this, dirty, dt);

			if(it->resting())
				continue;

			for(auto& search : searches)
			{
				if(search.pNode == it.get())
					continue;

				float l2 = (it->getPosition() - search.pNode->getPosition()).lengthSq();
				if(l2 <= search.radiusSq)
				{
					float v = (l2 / search.radiusSq) + (search.pNode->getCharge() - it->getCharge()) * 0.25f;
					if(v < search.value || search.pHit == nullptr)
					{
						search.value = v;
						search.pHit = it.get();
					}
				}
			}
		}
	}

	void Cluster::update(std::vector<NodeInstance>& nodeInstances, UINT& currentIndex)
	{
		for(auto& node : m_nodes)
		{
			node->update(nodeInstances, currentIndex++);
		}
	}
}

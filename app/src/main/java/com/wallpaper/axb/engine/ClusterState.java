package com.wallpaper.axb.engine;

import java.io.DataOutputStream;
import java.io.IOException;

/**
 * Created by AxB on 2/24/2018.
 */

public class ClusterState {
    public String id;
    public Path<Floats.Float3> position = new Path<>(Floats.Float3.class);
    public Path<Floats.Float3> rotation = new Path<>(Floats.Float3.class);
    public Path<Floats.Float> offset = new Path<>(Floats.Float.class);
    public Path<Floats.Float> spread = new Path<>(Floats.Float.class);
    public Path<Floats.Float> yaw = new Path<>(Floats.Float.class);
    public Path<Floats.Float> pitch = new Path<>(Floats.Float.class);
    public Path<Floats.Float3> color = new Path<>(Floats.Float3.class);
    public Path<Floats.Float> size = new Path<>(Floats.Float.class);

    public ClusterState() {

    }

    public void write(DataOutputStream stream) throws IOException {

        // position
        position.write(stream);

        // rotation
        rotation.write(stream);

        // offset
        offset.write(stream);

        // spread
        spread.write(stream);

        // yaw spread
        yaw.write(stream);

        // pitch spread
        pitch.write(stream);

        // color
        color.write(stream);

        // size
        size.write(stream);
    }

    public static ClusterState createRandomState() {

        ClusterState state = new ClusterState();
        state.position.randomize(new Floats.Float3(-20, -20, -20), new Floats.Float3(20,20,20), 1, 3);
        state.rotation.randomize(new Floats.Float3(0, 0, 0), new Floats.Float3(360,360,360), 2, 5);
        state.offset.randomize(new Floats.Float(0), new Floats.Float(200), 1, 3);
        state.spread.randomize(new Floats.Float(10), new Floats.Float(100), 1, 3);
        state.color.randomize(new Floats.Float3(0, 0, 0), new Floats.Float3(1,1,1), 2, 4);
        state.size.randomize(new Floats.Float(0.5f), new Floats.Float(5f), 2, 4);
        state.yaw.randomize(new Floats.Float(0), new Floats.Float(180), 1, 3);
        state.pitch.randomize(new Floats.Float(0), new Floats.Float(180), 1, 3);
        return state;
    }

    public static ClusterState createHourglass() {
        ClusterState state = new ClusterState();
        state.position.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,-100,0),
                new Floats.Float3(0,100,0)
        });
        state.rotation.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,0,0)
        });
        state.offset.add(0.5f, new Floats.Float[]{
                new Floats.Float(0.0f),
                new Floats.Float(50.0f),
                new Floats.Float(50.0f),
                new Floats.Float(50.0f),
                new Floats.Float(0.0f)
        });
        state.offset.add(0.5f, new Floats.Float[]{
                new Floats.Float(0.0f),
                new Floats.Float(50.0f),
                new Floats.Float(50.0f),
                new Floats.Float(50.0f),
                new Floats.Float(0.0f)
        });
        state.spread.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f)
        });
        state.color.add(0.5f, new Floats.Float3[]{
                new Floats.Float3(1,0,0)
        });
        state.color.add(0.5f, new Floats.Float3[]{
                new Floats.Float3(0,0,1)
        });
        state.size.add(1.0f, new Floats.Float[]{
                new Floats.Float(2.0f),
                new Floats.Float(1.0f),
                new Floats.Float(2.0f)
        });
        state.yaw.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f)
        });
        state.pitch.add(1.0f, new Floats.Float[]{
                new Floats.Float(360.0f)
        });

        return state;
    }

    public static ClusterState createAxB() {
        ClusterState state = new ClusterState();
        state.position.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(-150,-100,0),
                new Floats.Float3(-100,100,0)
        });
        state.position.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(-50,-100,0),
                new Floats.Float3(-100,100,0)
        });
        state.position.add(0.25f, new Floats.Float3[]{
                new Floats.Float3(-125,0,0),
                new Floats.Float3(-75,0,0)
        });

        state.position.add(0.5f, new Floats.Float3[]{
                new Floats.Float3(-50,-100,0),
                new Floats.Float3(50,0,0)
        });
        state.position.add(0.5f, new Floats.Float3[]{
                new Floats.Float3(50,-100,0),
                new Floats.Float3(-50,0,0)
        });

        state.position.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(50,-100,0),
                new Floats.Float3(50,100,0)
        });
        state.position.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(50,100,0),
                new Floats.Float3(100,100,0),
                new Floats.Float3(100,50,0),
                new Floats.Float3(100,0,0),
                new Floats.Float3(50,0,0)
        });
        state.position.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(50,0,0),
                new Floats.Float3(150,0,0),
                new Floats.Float3(150,-50,0),
                new Floats.Float3(150,-100,0),
                new Floats.Float3(50,-100,0)
        });

        state.rotation.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,0,0)
        });

        state.offset.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f)
        });

        state.spread.add(1.0f, new Floats.Float[]{
                new Floats.Float(5.0f)

        });

        state.color.add(2.25f, new Floats.Float3[]{
                new Floats.Float3(1,0,0)
        });
        state.color.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,1,0)
        });
        state.color.add(3.0f, new Floats.Float3[]{
                new Floats.Float3(0,0,1)
        });

        state.size.add(1.0f, new Floats.Float[]{
                new Floats.Float(3.0f)
        });

        state.yaw.add(1.0f, new Floats.Float[]{
                new Floats.Float(360.0f)
        });
        state.pitch.add(1.0f, new Floats.Float[]{
                new Floats.Float(360.0f)
        });

        return state;
    }

    public static ClusterState createSpiral() {
        ClusterState state = new ClusterState();
        state.position.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,-100,0),
                new Floats.Float3(0,100,0)
        });
        state.rotation.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,0,0),
                new Floats.Float3(0,720,0)
        });
        state.offset.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f),
        });
        state.spread.add(1.0f, new Floats.Float[]{
                new Floats.Float(20.0f)
        });
        state.color.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(1,0,0),
                new Floats.Float3(0,1,0),
                new Floats.Float3(0,0,1)
        });
        state.size.add(1.0f, new Floats.Float[]{
                new Floats.Float(2.0f),
                new Floats.Float(3.0f),
                new Floats.Float(2.0f),
                new Floats.Float(3.0f),
                new Floats.Float(2.0f),
        });
        state.yaw.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f)
        });
        state.pitch.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f)
        });

        return state;
    }

    public static ClusterState createPP() {
        ClusterState state = new ClusterState();
        state.position.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(-100,-25,0),
                new Floats.Float3(-100,25,0)
        });
        state.rotation.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,0,0)
        });
        state.offset.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f),
        });
        state.spread.add(1.0f, new Floats.Float[]{
                new Floats.Float(50.0f)
        });
        state.color.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(1,1,0),
                new Floats.Float3(0,1,1),
        });
        state.size.add(1.0f, new Floats.Float[]{
                new Floats.Float(2.0f),
        });
        state.yaw.add(1.0f, new Floats.Float[]{
                new Floats.Float(360.0f)
        });
        state.pitch.add(1.0f, new Floats.Float[]{
                new Floats.Float(360.0f)
        });

        state.position.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(-100,25,0),
                new Floats.Float3(25,25,0),
        });
        state.color.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,1,1)
        });

        return state;
    }
}

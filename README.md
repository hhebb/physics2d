# Physics2D Simulator

> Personal Project

## About
- Simple and basic Physics Engine 2D Simulator.
- Simulator for Reinforce Learning or other Physical Environment Analysis.


### Environment

- Ubuntu 20.04 LTS
- g++
- Qt5
- OpenGL

### dependencies
```
sh dependencies.sh
```

### Build

```
cd ./src
cmake CMakeLists.txt -B build
cd build
make physics

```

## ScreenShots

> Chain (revolute joint)

![](/images/chain.gif)

> Free fall

![](/images/fall.gif)

> Pendulum (revolute joint)

![](/images/pendulum.gif)

> Iteration change result

![](/images/iteration.gif)


### Function

- Basic Dynamics, Collision, Joint physics operation.
- Step by Step physics simulate.
- Pause / Restart simulation.
- Change some physics pipeline property
    - Velocity Collision Iterator.
    - Position Collision Iterator.
    - Velocity JointIterator.
    - Velocity Joint Iterator.

## Detatil


### Used Theory

- RigidBody Dynamics
    - Apply Force.
    - Velocity Calculation.
    - Position Calculation.
- Collision Detection
    - GJK algorithm for detect collide or not.
    - EPA algorithm for collision information (how deep?, which direction?).
    - Clipping algorithm for collision manifolds (where is collision points?).
- Collision Handle, Joint  Handle
    - Constraint method.
        - All collision, joint, frictions are regarded as constrained condition.
        - Dynamics equation should fulfill constrained dynamics.
    - Iteration
        - For fast, stable body simulation.
        - Re-Calculate for all physical objects.

### Used Library

- Qt
- GL

### Architecture

- class diagram

## Further Work


- Function
    - Transfer single step physics property to other module.
    - Interfere physics module, add command and apply.
    - Accept command from User, external modules.
- Module
    - Reinforcement Learning Module
- New Version
    - 3D Physics Engine

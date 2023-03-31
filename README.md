# Physics2D Simulator for RL

> 2D physics engine for simulation & Reinforcement Learning Environment.

## About
- Simple and basic Physics Engine 2D Simulator.
- Simulator for Reinforce Learning or other Physical Environment Analysis.
- Link to python RL (e.g. Gymnasium) library using Pybind11.


### Environment

- Ubuntu 20.04 LTS
- g++
- Qt5
- OpenGL


### install dependencies
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

## C++ Physics Engine Simulation Example (with legacy OpenGL)
> Chain (revolute joint)

![](/images/chain_1.gif)

> Free fall

![](/images/fall_2.gif)
![](/images/fall_3.gif)

> Cart Pole

![](/images/cartpole.gif)

> snake

![](/images/snake.gif)


### Function

* Physics Simulation
    - Basic Dynamics, Collision, Joint physics operation.
    - Step by Step physics simulate.
    - Pause / Restart simulation.
    - Change some physics pipeline property
        - Velocity Collision Iterator.
        - Position Collision Iterator.
        - Velocity JointIterator.
        - Velocity Joint Iterator.


## Python RL Rendering (with modern OpenGL)
* C++ physics info --> python window.

> cartpole
![](/images/gym_cartpole.gif)

> snake
![](/images/gym_snake.gif)


### Bind to Python
* Support Reinforcement Learning Library (Gymnasium) link.
* Pybind11 binding.


## Usage
* simulator
    
```
# run physics simulator GUI

cd build/src
./physics
```

* RL env
```
# run gymnasium environment

cd python/src/envs
python run.py
```


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
- Pybind11

### Architecture

- class diagram

## Further Work

- Function
    - Interfere physics module, add command and apply.
    - Accept command from User, external modules.
- New Version
    - 3D Physics Engine

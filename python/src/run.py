import gymnasium as gym
import envs
import numpy as np

env = gym.make('envs/cartpole-v0')
s, info = env.reset()

while True:
    a = -1.0 if np.random.random() > .5 else 1.0
    s, r, d, info = env.step([a])
    print(s, r, d, info)
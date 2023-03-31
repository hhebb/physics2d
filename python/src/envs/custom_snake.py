import numpy as np
import gymnasium as gym
from gymnasium import spaces
import random
import env

from gymnasium.envs.registration import register

register(
     id="envs/snake-v0",
     entry_point="envs.custom_snake:CustomSnakeEnv",
    #  max_episode_steps=300,
)

class CustomSnakeEnv(gym.Env):
    metadata = {"render_modes": ["human", "rgb_array"], "render_fps": 4}

    def __init__(self, render_mode=None, size=5):
        self.observation_space = spaces.Dict(
            {
                "horizontal": spaces.Box(-1, 1, shape=(1,), dtype=float),
            }
        )

        self.action_space = spaces.Discrete(3)

        assert render_mode is None or render_mode in self.metadata["render_modes"]
        self.render_mode = render_mode

        self.window = None
        self.clock = None

        self.environment = env.Environment('snake')


    def _get_info(self):
        return {
            "distance": np.linalg.norm(
                self._agent_location - self._target_location, ord=1
            )
        }


    def reset(self, seed=None, options=None):
        # We need the following line to seed self.np_random
        super().reset(seed=seed)

        # get bodies
        bodies = self.environment.GetBodies()

        # re-form observation
        pos = bodies[0].GetPosition().x
        observation = {'horizontal': pos}
        
        info = {'info': None}

        return {'obs': observation, 'info': info}


    def step(self, action):
        self.environment.Step(action)
        # if self.render_mode == "human":
        #     self._render_frame()

        # get bodies
        bodies = self.environment.GetBodies()

        # re-form observation
        pos = bodies[0].GetPosition().x
        observation = {'horizontal': pos}

        # reward
        if pos < -0.5:
            reward = -200
        elif pos > 1:
            reward = 200
        else:
            reward = 0

        # is done
        done = True if pos < -0.5 or pos > 1 else False

        # info
        info = {'info': None}

        return observation, reward, done, info


    def close(self):
        if self.window is not None:
            pass
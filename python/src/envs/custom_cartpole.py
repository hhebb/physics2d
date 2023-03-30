import numpy as np
import gymnasium as gym
from gymnasium import spaces
import random
import env

from gymnasium.envs.registration import register

register(
     id="envs/cartpole-v0",
     entry_point="envs.custom_cartpole:CustomCartpoleEnv",
    #  max_episode_steps=300,
)

class CustomCartpoleEnv(gym.Env):
    metadata = {"render_modes": ["human", "rgb_array"], "render_fps": 4}

    def __init__(self, render_mode=None, size=5):
        self.observation_space = spaces.Dict(
            {
                "horizontal": spaces.Box(-1, 1, shape=(1,), dtype=float),
                "angle": spaces.Box(-180, 180, shape=(1,), dtype=float)
            }
        )

        self.action_space = spaces.Discrete(2)

        assert render_mode is None or render_mode in self.metadata["render_modes"]
        self.render_mode = render_mode

        self.window = None
        self.clock = None

        self.environment = env.Environment('cartpole')


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
        angle = bodies[2].GetRotation()
        pos = bodies[0].GetPosition().x
        observation = {'horizontal': pos, 'angle': angle}
        
        info = {'info': None}

        return {'obs': observation, 'info': info}


    def step(self, action):
        self.environment.Step(action)
        # if self.render_mode == "human":
        #     self._render_frame()

        # get bodies
        bodies = self.environment.GetBodies()

        # re-form observation
        angle = bodies[2].GetRotation()
        pos = bodies[0].GetPosition().x
        observation = {'horizontal': pos, 'angle': angle}

        # reward
        reward = -200 if angle > 60 or pos > 1 or pos < -1 else 1

        # is done
        done = True if angle > 60 or pos > 1 or pos < -1 else False

        # info
        info = {'info': None}

        return observation, reward, done, info


    def close(self):
        if self.window is not None:
            pass
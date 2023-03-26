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

        self.environment = env.Environment()


    def _get_info(self):
        return {
            "distance": np.linalg.norm(
                self._agent_location - self._target_location, ord=1
            )
        }


    def reset(self, seed=None, options=None):
        # We need the following line to seed self.np_random
        super().reset(seed=seed)

        observation = self.environment.GetEnvState()
        info = self.environment.GetEnvInfo()

        return {'obs': observation, 'info': info}


    def step(self, action):
        self.environment.Step(action)
        observation = self.environment.GetEnvState()
        reward = self.environment.GetEnvReward()
        done = self.environment.GetEnvIsDone()
        info = {'info': self.environment.GetEnvInfo()}

        # if self.render_mode == "human":
        #     self._render_frame()

        # re-forming
        observation = {'angle': observation[1], 'horizontal': observation[0]}

        return observation, reward, done, info


    def close(self):
        if self.window is not None:
            pass

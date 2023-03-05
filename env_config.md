{
    "TEST_NAME": "basic revolute",
    "BODIES": [
      {
        "ID": 0,
        "NAME": "BASE",
        "TYPE": 2,
        "LAYER": 0,
        "VERTICES": [
          "0, 0",
          "1.0, 0.0",
          "1.0, 0.1",
          "0.0, 0.1"
        ],
        "POSITION": "0.0, 0.5",
        "ROTATION": 0
      },
      {
        "ID": 1,
        "NAME": "TOP",
        "TYPE": 0,
        "LAYER": 0,
        "VERTICES": [
          "0, 0",
          "0.5, 0.0",
          "0.5, 0.1",
          "0.0, 0.1"
        ],
        "POSITION": "0.0, 0.0",
        "ROTATION": 60
      }
    ],
    "JOINTS": [
      {
        "TYPE": "REVOLUTE",
        "BODY_A": 0,
        "BODY_B": 1,
        "OFFSET_A": "0.0, 0.0",
        "OFFSET_B": "0.5, 0.0"
      }
    ],
    "ACTIONS": [
      {
        "BODY": 1,
        "TYPE": "DISCRETE",
        "POINT": "0.0, 0.3",
        "DIRECTION": "0.1, 0.3",
        "MAGNITUDE": 1

      }
    ],
    "STATES": [

    ],
    "REWARD": [
      
    ]
  }
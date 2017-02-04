#! /bin/bash

v4l2-ctl -c brightness=30,contrast=10,exposure_auto=1,exposure_absolute=5,saturation=200,white_balance_temperature_auto=0,sharpness=50,white_balance_temperature=6500
frcImageProcessing &


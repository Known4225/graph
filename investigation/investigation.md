Recorded data using:
```
> python sipmBreakdown.py sipmPrint.txt sipmLog.txt COM3 xml\MeasurementLidarPSSI_100_us_59235.xml 4000 2550 10
```

Recorded six scenarios:
- Open air
- Under black sheet
- Apature Open air
- Apature and Receiver Lens Holder Open air
- Apature and Receiver and Foam Open Air
- Receiver and Foam, No apature

A picture of the setup is included for each scenario as is a picture of the graph. The graphs are the raw data.

# Notes
- Neither the lens holder nor the apature had a filter attached
- The laser was turned off during each sweep
- Each sweep was taken from a DAC value of 4000 to 2570, with a step size of 10.
- Each sweep was performed on the same board
- Calculated Breakdown Voltage, ~3160 or 36.44V (using \\r2d2\users\!NPD\Active\NPD_2683375_LiDAR Amazon IXD\System Info\Theory of Operation\tof calculator.xlsx)
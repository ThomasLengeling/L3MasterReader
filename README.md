# L3MasterReader

The MasterReader program is a C++ application that was designed to recognize and analyze physical grid cells of the Cityscope platform. The application takes advantage of openCV to accomplish the detection of Aruco tags in real time. The data is compressed into a string before being sent via the UDP protocol to a host, either remotely or locally.

Checkout Branch Simple for single use camera input

### Keys

- Key: '1' Raw View of cameras
- Key: "2" Perspective View

           -   Key: 'l' iterate through corners
           -   Key: 'UP' "DONW" "LEFT" "RIGHT" to manually move each corner of the perspective view
           -   Key: "SHIFT" + mouse pressed to move the corners using the mouse pointer
           
- Key: '3' Detection dots

            -  Key: "SHIFT" + Mouse pressed to add point with id 'i' for detection.
            -  Key: 'z' increase by 1 the iterator to add a dot 'i'
            -  Key: 'x' decrease by 1 the iterator to add a dot 'i'
          
- Key: '4 - Color correction Mode, Increase or decrease the image contrast and saturation wit the GUI
- Key: '5' Release Mode
- Key: 's' save modifications


### Files:

- img.json Camera information
          - Upadate the ``camId`` to correspond the 'id' of the input camera
```
[
  {
    "cam0": {
      "alpha": 0.6499999761581421,
      "beta": 27.209999084472656,
      "camId": 0,
      "gamma": 1.2999999523162842,
      "x1": 0,
      "x2": 0,
      "y1": 1920,
      "y2": 1080
    }
  }
]
```
- gridpos_00 Dots positions

- griddef.json Grid definition

Update the x and y values to correspond the dected grid area
```
{
  "grid_0": {
    "x": 16,
    "y": 16
  }
}
```

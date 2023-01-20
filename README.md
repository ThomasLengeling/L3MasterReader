# L3MasterReader

Checkout Branch Simple for single use camera input

### Keys

- Key: '1' - Raw View of cameras
- Key: "2" - Perspective View
           - Key: 'l' iterate through corners
           - Key: 'UP' "DONW" "LEFT" "RIGHT" to manually move each corner of the perspective view
           - Key: "SHIFT" + mouse pressed to move the corners using the mouse pointer
- Key: '3' Detection dots
         - Key: "SHIFT" + mouse pressed to add point with id 'i' for detection. The id 'i' is automatically increase by 1 once a dot is added.
         - Key: 'z' increase by 1 the iterator to add a dot 'i'
         - Key: 'x' decrease by 1 the iterator to add a dot 'i'
- Key: '4 - Color correction Mode
          - Increase or decrease the image contrast and saturation wit the GUI
- Key: '5' Release Mode
- Key: 's' save modifications


### Files:

- img.json Camera information
          - Upadate the 'camId' to correspond the 'id' of the input camera
- gridpos_00 Dots positions
- griddef.json Grid definition

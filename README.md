# v4l2_front_end
(WIP) This V4L2 device will not be developed any furter as this hardware block needs to be implemented in a drm/kms module.

Notes:
Some setup was done in u-boot, the clock for the front-end is setup here, so there is one missing config.
No IRQ
Important: see sunxi_front-end:601.

The manually added IOCTL are stale. These were added as a starting point for
using the Allwinner A20 Display Engine front end.
So ignore the msleep(100);

Hopes this helps anyone.

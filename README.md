# AdvancedUMG
A plugin for UE4 that adds extended UMG functionality without having to patch the engine

Most of the inspiration for this plugin was taken from CSS and Godot Engine node hierarchy.   
This plugin was initially designed to be used for mobile devices, but it's been updated to support desktop platforms as well.

### Adv Panel
The most basic element without any functionality. Serves as a container to store multiple child elements in. Child elements can change their padding and alignment properties.

![image](https://user-images.githubusercontent.com/27584451/155496182-942d06ca-0b1a-4d37-b5d0-aed514f256a0.png)

### Adv Padding Panel
Same as Adv Panel, but lets you set a common padding value for all child elements.


https://user-images.githubusercontent.com/27584451/155495633-5103f5eb-d79b-492e-84f8-f601413adf19.mp4


### Adv Horizontal/Vertical Box
An extended version of regular Horizontal/Vertical Box, with an addition of letting you set common spacing between child elements. More ways of organizing child elements may be added in the future.


https://user-images.githubusercontent.com/27584451/155494917-131aae76-be3c-4252-95f2-0eda29bf3019.mp4


### Adv Image
Same as a regular Image, but it lets you put multiple child elements inside. Follows the same rules as Adv Panel.

![image](https://user-images.githubusercontent.com/27584451/155499470-57dde08d-1547-4c2c-9e00-cbdcf445b8a0.png)

### Adv Generic Button
A copy of the regular button. Lets you place multiple child elements inside. Follows the same rules as Adv Panel.

![image](https://user-images.githubusercontent.com/27584451/155501624-2d07b691-b808-47ed-aefe-e5e80f5c490c.png)

### Adv Simple Button
A simplified version of the regular button. It uses only one image, nudges all content down including itself when pressed. Nudge amount can be modified. Lets you place multiple child elements inside. Follows the same rules as Adv Panel.

https://user-images.githubusercontent.com/27584451/155502340-98c50848-c73c-4466-b68b-e5bd6ba743c9.mp4


### Adv Color Rect
A simple element that lets you paint a solid color. Optionally lets you paint a solid color outline with modifiable width. Lets you place multiple child elements inside. Follows the same rules as Adv Panel.

https://user-images.githubusercontent.com/27584451/155502824-6af9a8f7-753c-47b4-8a5e-768e03139f88.mp4


### Adv Zero Size Panel
Advanced container element that always reports its desired size as zero. Useful when the size of child elements shouldn't be accounted for in the parent element. Follows the same rules as Adv Panel.


https://user-images.githubusercontent.com/27584451/155506085-a1d7a42b-bf7d-403a-a001-43d4ec4cc986.mp4

### Adv Border
Exists solely for the purpose of drawing a border outline from a brush. The major difference between the normal "Draw As: Border" mode in a brush is that the resulting image is scaled with screen size. Also lets you place multiple children inside.

https://user-images.githubusercontent.com/27584451/155508186-8aaeb60e-43a1-4ccc-9a12-88f80af2113f.mp4


### Adv Bottom Sheet
See [example of a bottom sheet on material.io](https://material.io/components/sheets-bottom). Designed to be used with mobile devices. Attempts to resemble a regular bottom sheet as close as possible.

https://user-images.githubusercontent.com/27584451/155510527-ef96a59b-09ed-4955-a4b8-4ef2dc483c80.mp4

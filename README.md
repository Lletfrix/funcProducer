# funcProducer - An Image to C-function converter

This script turns a png image to a C-function

## Getting Started

Please, understand C is limited in the graphically talking, so please consider some limitations.
Usually, a resolution of a gba (240*160 px) is fine for most purposes, if you only want to print one static image then you can increase the resolution.

### Prerequisites

There are a few things that are required:

ImageMagick. To install it just open your console and type:

```
sudo apt-get install imagemagick
```

Any program that lets you modify your images and index them. GIMP is a good one:

```
sudo apt-get install gimp
```

You must configure your Linux terminal to show custom colours. Also you should change your font-size to a very little value (from 2 px to 4 px) in order to display the pixelArt properly.

The colour in left-to-right order are (the order matters):
```
140C1C
442434
30346D
4E4A4E
854C30
346524
D04648
757161
597DCE
D27D2C
8595A1
6DAA2C
D2AA99
6DC2CA
DAD45E
DEEED6
```

Finally you should be getting something like this:
![Alt text](/Source/PalettePreview/preview.png?raw=true "Optional Title")

### Installing

If this is your first time running this program you have to compile source code and give permissions to the scripts. Type

```
sudo chmod +x ./install.sh
./install.sh
```
or 

```
sudo chmod +x ./funcProducer.sh
gcc -o img2fun ./Source/img2c.c
```

Your binaries should have been created. Now you need to copy Dawnbringer16.gpl to your gimp palettes directory, assuming you have GIMP 2.8 you should try:

```
sudo chmod +x ./gimpPalette.sh
./gimpPalette.sh
```
If that's not the case, just copy the content of ./palette to the palette directory of gimp, should be something like gimp-(ver)/palettes


## Usage

To use the program the image needs to be indexed with the given palette and withouth an Alpha Channel. If you know how to do this, you can skip next step.

### Indexing and removing alpha-channel in GIMP

Open your image in GIMP. Go to:

```
Image → Mode → Indexed
```
A menu will open, click on "Use custom palette"
Click on the left palette button and a new box will open. Look for Dawnbringer16. Click on it and let everything else as default. Click on convert.

Now you should make sure there is not an alpha-channel active. 
```
Layer → Transparency → Remove Alpha Channel
```

Now export as a png and you are ready to go!

### Converting your image, generating source code

Place your nameOfYourImage.png file on to "Pending" folder. Then open a terminal and type:

```
./funcProducer.sh nameOfYourImage
```
Notice how .png is not added to the sentence.

Now move to Produced folder and there should be a new directory with all your files.

Here you will encounter nameOfYourImage.c, nameOfYourImage_full.c, nameOfYourImage.txt, and nameOfYourImage.png

A little description about them:
	1) nameOfYourImage.c - This is the pure print function, not compilable.
	2) nameOfYourImage_full.c - This is the compilable program, will print your image.
	3) nameOfYourImage.txt - Information about each pixel of the image.
	4) nameOfYourImage.png - Your indexed alpha-free image.

### Printing my image

Execute next piece of code. 
Please, make sure your linux terminal is configured as in the Prerequisites section before executing the second one.
```
gcc -o nameOfYourImage nameOfYourImage_full.c
./nameOfYourImage
```

### Using transparencies in the image

If you want to use an "Alpha" channel open your image with GIMP, index it and remove alpha-channel, then go back to:
```
Image → Mode → RGB
```
Now paint the transparencies with a color that's not in the palette or is not pure. #fe00fe should work (#ff00ff won't). Export your image and go to previous step.

## Deployment

The function that's printed into the non-compilable file must be used with the header of the compilable program.
The function will recieve both starting points to print in the console, (1,1) as default. For more information, read the compilable file.

## Built With

* [ImageMagick](https://www.imagemagick.org/) - The program that's used to convert *.png into *.txt
* [GIMP](https://www.gimp.org/) - Image manipulation

## Authors

* **Rafael Sánchez** - *All code* - [Lletfrix](https://github.com/Lletfrix)

## Acknowledgments

* Thanks to Dawnbringer for posting his 16-colour palette online: [Dawnbringer](http://pixeljoint.com/forum/forum_posts.asp?TID=12795)
* This program started to make easy conversions from pixelart image to Linux terminal in order to use them in a C-Programmed videogame.
* The code is poorly written and probably not easily understandable.


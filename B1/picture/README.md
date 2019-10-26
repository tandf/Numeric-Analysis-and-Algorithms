# Face Match
Build with Visual Studio 2017
Tested in Win10
Use OpenCV 3.43 with contirb

## Usage
FaceMatch.exe --source=string --target=string --sourcepset=string --targetpset=string [options] ...
options:
      --path          directory path for images and points, relative path supported (string [=./])
  -s, --source        source image file name (string)
  -t, --target        source image file name (string)
  -1, --sourcepset    point set for source image (string)
  -2, --targetpset    point set for target image (string)
  -m, --method        interpolation method (string [=neighbour])
  -a, --all           show source image, target image, and generated images with three methods
  -p, --pointmark     mark points
  -d, --detect        detect points
  -?, --help          print this message

## Examples
### 8.jpg to 6.jpg
```console
./FaceMatch.exe -s 8.jpg -t 6.jpg -1 8.txt -2 6.txt -ap
```

### face detect
```console
./FaceMatch.exe -s test.jpg -t 6.jpg -1 a -2 a -apd
```

## Reference
[tanakh/cmdline: A simple command line parser for C++](https://github.com/tanakh/cmdline)
[amusi/opencv-facial-landmark-detection](https://github.com/amusi/opencv-facial-landmark-detection)

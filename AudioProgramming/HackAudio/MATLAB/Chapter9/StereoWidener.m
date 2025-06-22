clc; clear;
[in, fs] = audioread("flute.wav");
L = in(:, 1);
R = in(:, 2);

side = 0.5 * (L-R);
mid = 0.5 * (L+R);

width = 10;

sideNew = width*side;
midNew = (2-width) .* mid;

newLeft = mid + sideNew;
newRight = mid - sideNew;
 
out = [newLeft, newRight];

soundsc(out, fs);
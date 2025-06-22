clear; clc; close all;
%%
fs = 48000;
t = 0:1/fs:3;
t = t(:);
f = 100; phi = 0;

fn = sin(2*pi*f.*t + phi);
figure(1);
plot(t, fn);
%%
numOfSamples = 1*fs;
x = 2;
a = linspace(0, 1, numOfSamples); a = a(:);
fadeOut = 1 - a.^x;
a = linspace(1, 0, numOfSamples); a = a(:);
fadeIn = 1 - a.^x;

plot(a, fadeIn, a, fadeOut);
%%

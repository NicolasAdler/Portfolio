clear; clc;

fs = 48000;
t = 0:1/fs:3;
freq = 20;
gain = 0.5;

x = audioread('flute.wav');
%x = x(:,1);

N = length(x);
y = zeros(N,2);

for n = 1:N
    y(n,:) = x(n,:) * gain;
end

if y == x
    disp('x is equal to y');
else
    disp('x is not equal to y');
end

clear; clc;
%% Simple Demo
fileName = "flute.wav";
[sig, fs] = audioread(fileName);
sig = sig(:);

panValue = 100;

rightAmp = (panValue/200) + 0.5;
leftAmp = 1 - rightAmp;

stereoSig = [leftAmp*sig, rightAmp * sig];

soundsc(stereoSig, fs);
%% Using the Function
fileName = "flute.wav";
[sig, fs] = audioread(fileName);
pan_flute = pan(sig, -100, 1);
soundsc(pan_flute, fs);

%%
clc; clear; 
[in,fs] = audioread('flute.wav'); 

N = length(in); Ts = 1/fs; 

t = (0:N-1)*Ts; t = t(:);

f = 1; 

panValue = -100*sin(2*pi*f*t); 

panType = 2; % Start with panType = 2, but try 1,2,3,4 
[out] = pan(in,panValue,panType); 
out = out(:, 1:2);
soundsc(out, fs);

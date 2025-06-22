%% These next few examples begin our journey into making sine waves (oscillators)

f = 2;
phi = 0;
fs = 100;
Ts = 1/fs;
lenSec = 1;
N = fs*lenSec;

out1 = zeros(N,1);

for n = 1:N
    t = (n-1) * Ts;
    out1(n,1) = sin(2*pi*f*t+phi);
end

%% 
t = (0:(N-1)) * Ts;
t = t(:);
out2 = sin(2 * pi * f * t+pi/2);

%%
plot(t, out1, t, out2);



%%
[x, fs] = audioread('flute.wav');
N = length(x);
y = zeros(N,1);
d1 = fix(0.25 * fs);
b = 0.75;

for n = 1:N
    if n <d1+1 
        y(n,1) = x(n,1);
        % Why do we need this code?, Take a look at the following line
        % y(n,1) = x(n,1) + b* x(n-d,1);, without this code, how can we get
        % x(n-d,1)? we get negative samples???? Therfore if n is less than
        % d + 1, basically saying if n is less than the delay time, just
        % play the original audio, however when n is greater than the delay
        % time, then the code can begin to reach into already played
        % samples to replay them at a softer level for the delay echo
        % effect!
    else
        y(n,1) = x(n,1) + b* x(n-d1,1);
    end
end
soundsc(y,fs);
%%
[x, fs] = audioread('flute.wav');
N = length(x);
y = zeros(N,1);
d1 = fix(0.25 * fs);
a = -0.75;

for n = 1:N
    if n <d1+1 
        y(n,1) = x(n,1);
    else
        y(n,1) = x(n,1) - a* x(n-d1,1);
    end
end
soundsc(y,fs);
%%
[x, fs] = audioread('flute.wav');
N = length(x);
y = zeros(N,1);
feedback = zeros(N,1);
d1 = fix(0.5 * fs);
d2 = fix(0.75*fs);
a = 0.75;
b = 0.75;

for n = 1:N
    if (n <d1+1 || n < d2 + 1)
        y(n,1) = x(n,1);
    else
        y(n,1) = x(n,1) + b*x(n-d1,1) - a * y(n-d2,1);
    end
end
soundsc(y,fs);
%%



[x, fs] = audioread('flute.wav');
N = length(x);
y = zeros(N,1);
delay1 = fix(fs*0.47);
b1 = 0.75;
for n = 1:N
    if n<delay1+1
        y(n,1) = x(n,1);
    else
        y(n,1) = x(n,1) + b1 * y(n-delay1);
    end
end
soundsc(y,fs);

%%
[x, fs] = audioread("flute.wav");
N = length(x);
b =  0.75;
d = fix(0.45 * fs);
y = zeros(N,1);
tic
for n = 1:N
    if n <d+1
        y(n,1) = x(n,1); % just use original audio until the delay time is reached 
    else
        y(n,1) = x(n,1) + b*y(n-d,1); %feed back
        %y(n,1) = x(n,1) + b*x(n-d,1);
    end

end
toc
soundsc(y,fs);

%% Lets try with a CB
[x, fs] = audioread("flute.wav");
N = length(x);
b =  0.5;
d = round(0.45 * fs);
cb_size = round(1 *fs);
cb = zeros(cb_size,1);
y = zeros(N,1);
write = 1;
read = write-d;
tic
for n = 1:N
    if (read <1)
        read = read + cb_size;
    end
    if (n<d+1)
        y(n,1) = x(n,1);
    else
    cb(write,1) = x(n,1); % set the cb to the input
    y(n,1) = cb(write,1) + b*cb(read,1) + b*y(n-d,1);
    write = mod(write, cb_size) + 1;
    read = mod(read, cb_size) + 1;
    end
end
toc
soundsc(y, fs)

%%
[x,fs] = audioread("flute.wav");
N = length(x);
cb_size = round(fs*3);
y = zeros(N,1);
cb = zeros(N,1);
b = 0.4;
d = round(0.5*fs);
write = 1;
read = write -d;

for n=1:N
    if (read <1)
        read = read+cb_size;
    end
    if (n<d+1)
        y(n,1) = x(n,1);
    else
        cb(write,1) = x(n,1);
        y(n,1) = cb(write,1) + b*cb(read,1) + b*y(n-d,1);
        write = mod(write, cb_size) + 1;
        read = mod(read, cb_size) + 1;
    end
end
soundsc(y,fs);























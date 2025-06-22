[x,fs] = audioread("flute.wav");
zero_pad = zeros(4*fs,2);
x = [x;zero_pad];
N = length(x);
cb_size = round(fs*3);
y = zeros(N,1);
cb = zeros(cb_size,1);
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
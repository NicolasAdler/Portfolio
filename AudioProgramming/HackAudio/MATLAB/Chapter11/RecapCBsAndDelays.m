[x, fs] = audioread("flute.wav");
N = length(x);
y = zeros(N,2);
duration = 1.0;
delay = round(fs*duration);
cb = zeros(3*fs, 2);
b = 0.5;
%% feed forward stereo delay buffer
% for n = 1:N
%     for c = 1:2
%         if (n<delay+1)
%             y(n,c) = x(n,c);
%         else
%             y(n,:) = x(n,c) + b* x(n-delay,c);
%         end
%     end
% end
%% Simple stereo CB 
% write = 1;
% read = write - delay;
% if (read <1)
%     read = read+size(cb,1);
% end
% for n = 1:N
%     for c = 1:2
% 
%         if (n<delay+1)
%             y(n,c) = x(n,c);
%         else
%             cb(write,c) = x(n,c);
%             y(n,c) = cb(write,c) + b* cb(read,c);
%             read = mod(read, size(cb,c)) + 1;
%             write = mod(write, size(cb,c)) + 1;
%         end
%     end
% end

[x, fs] = audioread("flute.wav");
N = length(x);
y = zeros(N,2);
duration = 1.0;
delay = round(fs * duration);
cb = zeros(3 * fs, 2);  % 3s buffer to be safe
b = 0.5;

write = 1;

for n = 1:N
    read = write - delay;
    if read < 1
        read = read + size(cb, 1);
    end
    for c = 1:2
        cb(write, c) = x(n, c);
        y(n, c) = cb(write, c) + b * cb(read, c);
    end
    write = mod(write, size(cb,1)) + 1;
end

%% 
soundsc(y,fs);


modulations = [0.629960525	0.793700526	1	1.25992105	1.587401052	2	2.5198421	3.174802104	4	5.0396842	6.349604208	8	10.0793684	12.69920842]
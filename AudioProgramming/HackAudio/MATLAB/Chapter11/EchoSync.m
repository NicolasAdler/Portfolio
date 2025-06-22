[x,Fs] = audioread('flute.wav'); 
beatsPerMin = 102; 
beatsPerSec = beatsPerMin / 60;
secPerBeat = 1/beatsPerSec; 

noteDiv = 0.5 ; 
timeSec = noteDiv * secPerBeat; 

d = fix(timeSec * Fs);
b = 0.75;
N = length(x); 

for n = 1:N 
    if n < d+1 

        y(n,1) = x(n,1); 

    else 

    y(n,1) = x(n,1) + b*x(n-d,1); 

    end 

end 

sound(y,Fs); % Listen to the effect
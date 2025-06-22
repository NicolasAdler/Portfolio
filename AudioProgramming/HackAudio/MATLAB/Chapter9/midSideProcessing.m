[input, fs] = audioread("flute.wav");

left = input(:,1);
right = input(:,2);

mid = 0.5 * (left + right);
sides = 0.5 * (left - right);

newL = mid + sides;
newR = mid - sides;

output = [newL, newR];


soundsc(input, fs);



soundsc(output, fs);
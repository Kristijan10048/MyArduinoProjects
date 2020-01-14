clc;
fileName = 'temp_readings.txt';

%read text file with values
fileID = fopen(fileName,'r');

%define format 
formatSpec = '%f';

%load the data in to
tempValues = fscanf(fileID,formatSpec);

%close the file
fclose(fileID);

meanTemp = mean(tempValues);

minTemp = min(tempValues);
maxTemp = max(tempValues);

standard_dev = std(tempValues);

fprintf('Mean, %f\n', meanTemp);
fprintf('Standard deviaton, %f\n', standard_dev);
fprintf('Three sigma %f\n', 3*standard_dev);
fprintf('Min %f\n',minTemp);
fprintf('Max %f\n',maxTemp);

fprintf('Mmean - 3*sigma: %f\n',meanTemp - 3*standard_dev);
fprintf('Max + 3*sigma %f\n',meanTemp +3*standard_dev);

%plot(tempValues);

[Y2,K2,P2]=Kalman1D(tempValues,1);
f32=figure(1);set(f32,'Name','Testing 1: f(t)=4*sin(t/10)*exp(-t/200)+W, W~N(m,v)');
subplot(2,1,1), plot(tempValues,'g'), title(' noisy signal  (awgn)'), grid on;
subplot(2,1,2), plot(Y2,'r'), title(' Filtered signal,R=1e-2,Q=1e-5'), grid on;
mean(Y2);
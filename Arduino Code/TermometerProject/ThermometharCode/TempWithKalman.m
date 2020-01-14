clc;
fileName = 'temp_readings_with_kalman.txt';

%read text file with values
fileID1 = fopen(fileName,'r');

%define format 
formatSpec1 = '%f %f';

sizeA = [2 Inf];

%load the data in to
tempValues1 = importdata(fileName,' ', 2);

tempData = tempValues1.data(:,1);

filterdTempData = tempValues1.data(:,2);

%close the file
fclose(fileID1);

f32=figure(1);set(f32,'Name','Testing 1: f(t)=4*sin(t/10)*exp(-t/200)+W, W~N(m,v)');
subplot(2,1,1), plot(tempData,'g'), title(' noisy signal  (awgn)'), grid on;
subplot(2,1,2), plot(filterdTempData,'r'), title(' Filtered signal,R=1e-2,Q=1e-5'), grid on;

mean(filterdTempData)

std(filterdTempData)

std(tempData)
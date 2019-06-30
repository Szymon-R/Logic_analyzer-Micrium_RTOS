close all;
resolution=1000;%ms
baud=115200;
tim_res=1/(180*resolution);
baud=baud*2;
fileID = fopen('data.txt','r');
formatSpec = '%ld';
data = fscanf(fileID,formatSpec);
fclose(fileID);
time=zeros(length(data),1);
diff=zeros(length(data),1);
clock=0:1/baud:1/baud*(length(time)-1);
clock=clock.*(1000000/resolution);
clock_values=zeros(length(clock),1);


for i=1:length(data)
    data(i)=data(i)+i*offset;
    if data(i)<0
        time(i)=data(i)+2147483648;
        data(i)=0;
    else
        time(i)=data(i);
        data(i)=1;
    end
  
    time(i)=time(i)*tim_res;

    
end

clock=clock+time(1);

for i=1:length(clock)
    if mod(i,2)==0
        clock_values(i)=0.5;
    end
end


for i=1:length(data)-1

diff(i)=time(i+1)-time(i);
    if diff(i)>0.06
            diff(i)=0.03;
    end
    
    if diff(i)<0.002
        %    diff(i)=0.003;
    end
end
figure
plot(diff)
figure
hold on
stairs(time,data);
stairs(clock,clock_values);
ylim([-2 2]);
hold off


median(diff)
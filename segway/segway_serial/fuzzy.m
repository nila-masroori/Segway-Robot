try
    fclose(ss);
catch
end
clc; %clear all; close all;

global ss sumErr
ser = seriallist;
ss = serial(ser(1));
ss.BaudRate = 115200
fopen(ss);

%fis=readfis('fuzzypid');
fis=readfis('segwaycontroller');

out_max = 100.0;
scale = 20;
sumErr = 0;


angleOffset = 3.8;


sp = 0.0;
angle = 0;
err = 0;
lastErr = 0.1;
err_dot = 0.1;
err_dot_dot = 0.1;
lastErr_dot = 0.1;
u = 0.0;
u_dot = 0.0;
u_dot_dot = 0.0;
last_u = 0.0;
last_u_dot = 0.0;
u_z = 0;
u_zz = 0;
angle_z = 0;
angle_zz = 0;

dt = 0.02;


xx = [err , err_dot , err_dot_dot , u_dot , u_dot_dot];
yy = [u];
vec = [0;0;0;0;0;0;0;0;0;0;0;0;0;0];
tic
while true
   % tic
    inp = fread(ss,1);
    %inp = uint8(inp);
    vec = [vec;inp];
   
   while (length(vec) > 5)
        if (vec(1) == 0 && vec(6) == 255)
            %flushinput(ss)
            dt = double(toc)
            tic
                A = uint8(vec(2:5));
                angle = double(typecast( A , 'single')) + angleOffset
                err = sp - angle;
                
%                 x = [err , err_dot , err_dot_dot , u_z , u_zz];
                 
               % u = controllerfuzzy(fis,err_dot,err, dt);
               u=fuzzycontrol(fis,err_dot,err);
                err_dot = err;
                left_motor = u*scale;
                right_motor = u *scale;
                send_packet(ss,left_motor,right_motor);
               
        end
        vec = vec(2:end);
    end 
     
    %toc
end

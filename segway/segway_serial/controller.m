function y = controller(err , err_dot , dt)
global sumErr

kp = 6;
kd = 2;
ki = .1;
sumErr_max = 20.0;
diffErr_max = 20.0;
out_max = 100.0;

if (err*sumErr < 0)
    sumErr = 0.0;
end

sumErr = sumErr + err*dt;

if (sumErr > sumErr_max)
    sumErr = sumErr_max;
elseif (sumErr < -sumErr_max)
    sumErr = -sumErr_max;
end

if (err_dot > diffErr_max)
    err_dot = diffErr_max;
elseif (err_dot < -diffErr_max)
    err_dot = -diffErr_max;
end


out = err * kp + kd*err_dot + ki*sumErr;
y = out + sign(err)*2;

if (y > out_max)
    y = out_max;
elseif (y < -out_max)
    y = -out_max;
end

end

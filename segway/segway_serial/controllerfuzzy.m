function y = controllerfuzzy(fis,err_dot,err, dt)
global sumErr


I_Term_max = 20.0;
D_Term_max = 20.0;
out_max = 100.0;
% sumErr = sumErr + err;
kpmin=10;
kpmax=15;
kdmin=0.05;
kdmax=0.1;
% p = 0;
% d = 0;
% alpha = 1;
%[p, d, alpha]
A=evalfis([err,err_dot],fis);
p = A(1);
d = A(2);
alpha = A(3);


kp = p*(kpmax - kpmin) + kpmin
kd = d*(kdmax - kdmin) + kdmin ;
ki = kd./alpha ;

% kp = 10;
% kd = 0.1;
% ki = 0.2 ;
if (err*sumErr < 0)
    sumErr = 0.0;
end

sumErr = sumErr + err*dt;
err_dot = (err - err_dot)/dt ; 


P_Term = err * kp
I_Term = ki*sumErr
D_Term = kd*err_dot

I_Term = 0;
%D_Term=0;

if (I_Term > I_Term_max)
    I_Term = I_Term_max;
elseif (I_Term < -I_Term_max)
    I_Term = -I_Term_max;
end

if (D_Term > D_Term_max)
    D_Term = D_Term_max;
elseif (D_Term < -D_Term_max)
    D_Term = -D_Term_max;
end

out = P_Term + D_Term+ I_Term;
y = out ;

if (y > out_max)
    y = out_max;
elseif (y < -out_max)
    y = -out_max;
end

end

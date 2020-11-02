#q1
select Fname from EMPLOYEE where Super_ssn = ( select Ssn from EMPLOYEE where Fname='Jennifer' and Lname='Wallace');

#q2
select e.Fname from EMPLOYEE e,DEPENDENT d where e.Ssn=d.Essn and e.Fname = d.Dependent_name and e.Sex=d.Sex;

#q3
select e.Fname from EMPLOYEE e,DEPARTMENT d,WORKS_ON w where e.Super_ssn=d.Mgr_ssn and d.Dnumber=5 and w.Essn=e.Ssn and w.Hours > 10 and w.Pno = (select Pnumber from PROJECT where Pname='ProductX') ;

#q4
select p.Pnumber,p.Dnum,e.Lname,e.Address,e.Bdate from EMPLOYEE e,DEPARTMENT d,PROJECT p where p.Plocation='Stafford' and p.Dnum=d.Dnumber and d.Mgr_ssn=e.Ssn;

#q5
with pnums as (select Pnumber from PROJECT where Dnum=5),
temp1 as (select Essn,Pno from WORKS_ON,pnums where Pno=Pnumber),
temp2 as (select distinct Essn,Pnumber from temp1,pnums),
temp3 as (select Essn from temp2 t where (t.Essn,t.Pnumber) not in (select * from temp1))
select distinct e.Fname from EMPLOYEE e,temp1 t  where  e.Ssn=t.Essn and e.Ssn not in  (select * from temp3);





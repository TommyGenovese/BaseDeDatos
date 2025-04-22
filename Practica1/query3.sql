SELECT e.employeenumber,
       e.lastname
FROM   employees e
WHERE  e.reportsto IN (SELECT e.employeenumber
                       FROM   employees e
                       WHERE  e.reportsto IN (SELECT e.employeenumber
                                              FROM   employees e
                                              WHERE  e.reportsto IS NULL))  
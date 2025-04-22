SELECT o.officecode,
       Count(*)
FROM   offices o,
       employees e,
       customers c,
       orders ord,
       orderdetails od,
       products p
WHERE  od.ordernumber = ord.ordernumber
       AND p.productcode = od.productcode
       AND ord.customernumber = c.customernumber
       AND c.salesrepemployeenumber = e.employeenumber
       AND e.officecode = o.officecode
GROUP  BY o.officecode
ORDER  BY Count(*) DESC
LIMIT  1  
SELECT p.productline,
       Avg(o.shippeddate - o.orderdate) AS Tiempo_medio
FROM   products p,
       orderdetails od,
       orders o
WHERE  p.productcode = od.productcode
       AND o.ordernumber = od.ordernumber
GROUP  BY p.productline
ORDER  BY tiempo_medio
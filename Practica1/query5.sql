SELECT o.country,
       Count(o.officecode) AS offices_without_sales
FROM   offices o
       LEFT JOIN employees e
              ON o.officecode = e.officecode
       LEFT JOIN customers c
              ON e.employeenumber = c.salesrepemployeenumber
       LEFT JOIN orders ord
              ON c.customernumber = ord.customernumber
                 AND ord.orderdate >= '2003-01-01'
                 AND ord.orderdate <= '2003-12-31'
WHERE  ord.ordernumber IS NULL
GROUP  BY o.country
HAVING Count(o.officecode) > 1
ORDER  BY offices_without_sales DESC;  
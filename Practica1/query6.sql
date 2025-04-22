SELECT od1.productcode AS product1,
       od2.productcode AS product2,
       Count(*)        AS num_orders
FROM   orderdetails od1
       JOIN orderdetails od2
         ON od1.ordernumber = od2.ordernumber
            AND od1.productcode < od2.productcode
GROUP  BY od1.productcode,
          od2.productcode
HAVING Count(*) > 1
ORDER  BY product1 DESC;  
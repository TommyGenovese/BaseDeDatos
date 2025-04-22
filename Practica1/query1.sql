SELECT c.customernumber,
       c.customername,
       Sum(p.amount) AS pago_totale
FROM   customers c,
       payments p,
       orders o,
       orderdetails od,
       products pr
WHERE  pr.productcode = od.productcode
       AND od.ordernumber = o.ordernumber
       AND o.customernumber = c.customernumber
       AND pr.productname = '1940 Ford Pickup Truck'
GROUP  BY c.customernumber,
          c.customername
ORDER  BY pago_totale DESC  
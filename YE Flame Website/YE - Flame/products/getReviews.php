<?php

$server = "mysql.hostinger.co.uk";
$username = "u215184977_user";
$password = "T0ta11yR4nd0mz^o^";
$database = "u215184977_revie";

$conn = mysql_connect($server, $username, $password);

if (!$conn)
{
        die('Could not connect: ' . mysql_error());
}

mysql_select_db($database, $conn);

// Get the status from the last form submission, otherwise use default.
$product = $_GET['prodid'];

// Get all the results for issues matching the chosen status id.
$result = mysql_query("SELECT * FROM reviews WHERE id = " . $product, $conn);

// Get the name of the chosen status.
$chosen_status = mysql_query("SELECT name FROM issue_statuses WHERE id = " . $choice, $conn);
$status_name = mysql_fetch_array($chosen_status, MYSQL_ASSOC);

if (!$options || !$result || !$chosen_status)
{
        echo("<p>Error performing query: " . mysql_error() . "</p>");
        exit();
}

mysql_close($conn);

?>
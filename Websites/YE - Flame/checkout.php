<?php

include_once('jcart/jcart.php');
if (!isset($_SESSION)) {
  session_start();
}
?>

<!DOCTYPE html>
<html>
    
    <head>
        <meta charset="utf-8">
        
        <title>Flame - Checkout</title>
        
        <link rel="icon" type="image/png" href="icon.png">
        
        <link rel="stylesheet" type="text/css" href="styles/norm.css">
        
        <!-- JCart stylesheet -->
        <link rel="stylesheet" type="text/css" media="screen, projection" href="jcart/css/jcart.css" />
        
        <link rel="stylesheet" type="text/css" href="styles/main.css">
        <link rel="stylesheet" type="text/css" href="styles/font.css">
        <link rel="stylesheet" type="text/css" href="styles/checkout.css">
    </head>
    
    <body>
        <header>
            <div id="navbar">
                <div id="navbar-logo">
                    <img id="navbar-logo-image" src="logo.png"/>
                    <h1 id="navbar-logo-text">Flame</h1>
                </div>

                <ul id="navbar-links">
                    <li><a href="index.html">Home</a></li>
                    <li><a href="products.html">Products</a></li>
                    <li><a href="support.html">Support</a></li>
                    <li class="dropdown"><a href="#">About</a>
                        <ul>
                            <li><a href="about-us.html">About Us</a></li>
                            <br>
                            <li><a href="trade-fairs.html">Trade Fairs</a></li>
                            <br>
                            <li><a href="ye.html">Young Enterprise</a></li>
                        </ul>
                    </li>
                </ul>
            </div>
        </header>
        
        <div class=containerMain>
            <div class="checkoutContainer">
                <h1>Checkout</h1>
                <div id="content">
                    <div id="jcart"><?php $jcart->display_cart();?></div>

                    <p><a href="index.html">&larr; Continue shopping</a></p>

                    <?php
                        //echo '<pre>';
                        //var_dump($_SESSION['jcart']);
                        //echo '</pre>';
                    ?>
                </div>
            </div>
        </div>
        
        <footer>
            <div id="footer">
                <div id="footer-contact">
                    <h4>Contact Us</h4>
                        <div>
                            <h4>Sales</h4>
                            <p>marcus.green@abingdon.org.uk</p>
                            <h4>General</h4>
                            <p>flameye.enquiries@abingdon.org.uk</p>
                        </div>
                </div>
                <div id="footer-links">
                    <h4>Quick Links</h4>
                </div>
                <div id="footer-follow">
                    <ul>
                        <li><a href = "http://www.facebook.com/FlameYoungEnterprise"><span class="icon-facebook"></span></a></li>
                        <li><a href = "mailto:flameye.enquiries@gmail.com"><span class="icon-mail"></span></a></li>
                    </ul>
                </div>
            </div>
        </footer>

        
    </body>
    
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
    
    <!-- JCart javascript -->
    <script type="text/javascript" src="jcart/js/jcart.min.js"></script>

</html>
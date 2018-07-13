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
        
        <title>Flame - Folepuncher</title>
        
        <link rel="icon" type="image/png" href="../icon.png">
        
        <link rel="stylesheet" type="text/css" href="styles/norm.css">
        
        <!-- JCart stylesheet -->
        <link rel="stylesheet" type="text/css" media="screen, projection" href="../jcart/css/jcart.css" />
        
        <link rel="stylesheet" type="text/css" href="../styles/main.css">
        <link rel="stylesheet" type="text/css" href="../styles/font.css">
        <link rel="stylesheet" type="text/css" href="../styles/products.css">
    </head>
    
    <body>
        <header>
            <div id="navbar">
                <div id="navbar-logo">
                    <img id="navbar-logo-image" src="../logo.png"/>
                    <h1 id="navbar-logo-text">Flame</h1>
                </div>

                <ul id="navbar-links">
                    <li><a href="../index.html">Home</a></li>
                    <li><a href="../products.html">Products</a></li>
                    <li><a href="../support.html">Support</a></li>
                    <li class="dropdown"><a href="#">About</a>
                        <ul>
                            <li><a href="../about-us.html">About Us</a></li>
                            <br>
                            <li><a href="../trade-fairs.html">Trade Fairs</a></li>
                            <br>
                            <li><a href="../ye.html">Young Enterprise</a></li>
                        </ul>
                    </li>
                </ul>
                
                <div id="shop">
                    <a href="../checkout.php"><span class="icon-WheelEdition"></span></a>
                </div>
            </div>
        </header>
        
        <div class=containerMain>
            <div class="productContainer">
                <div id="image-container"><img src="upcycling.png"/></div>
                <div class="desc">
                    <h1>Bolders</h1>
                    <p>Watch out! They're here!</p>
                        <p>For public safety, we have decided to publish a list of items that it may devour:</p>
                        <ul>
                            <li>Phones of virtually any sort (it can use a charging lead to charge the phone while eating it, making it especially deadly)</li>
                            <li>Keys</li>
                            <li>Small items left on floors</li>
                            <li>Money</li>
                            <li>Photos</li>
                            <li>Credit cards</li>
                            <li>Letters</li>
                            <li>CDs</li>
                            <li>Pencils</li>
                            <li>Leads</li>
                            <li>Leaflets</li>
                            <li>Rubbers</li>
                            <li>Bottles</li>
                            <li>Fingers</li>
                            <li>Almost anything</li>
                        </ul>
                        <p>
                            They are bred in the United Kingdom in Oxfordshire and are mutated from infected (and only high-quality) tennis balls. It has a strong grip and on its back, a suction pad is located to enable it to hold objects for days in the same place without moving an inch and allow it to club and can therefore be found in tall and high places. 
                        </p>
                    
                    <form method="post" action="" class="jcart">
                        <fieldset>
                            <input type="hidden" name="jcartToken" value="<?php echo $_SESSION['jcartToken'];?>" />
                            <input type="hidden" name="itm-id" value="2" />
                            <input type="hidden" name="itm-name" value="Bolder" />
                            <input type="hidden" name="itm-price" value="3.00" />
                            <input type="hidden" name="itm-url" value="upcycling.php" />

                            <div class="amount">
                                <label>Quantity:</label>
                                <br>
                                <input name="itm-qty" type="number" min="0" placeholder="Amount" onchange="updatePrice(this.value)">
                                
                                <input name="add-button" type="submit" class="btn" value="Add to cart">
                            </div>

                            <div class="price">
                                <label>Price:</label>
                                <br>
                                <label class="price" id="price">£0.00</label>
                            </div>
                        </fieldset>
                    </form>

                </div>
                
                <br class="clear"/>
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
    <script type="text/javascript" src="../jcart/js/jcart.min.js"></script>
    
    <script>
        function updatePrice(val) {
            var price = document.getElementsByName("itm-price")[0].value;
            document.getElementById("price").innerHTML = "£"+(Math.max(0,parseInt(val)*price)).toFixed(2);
        }
        
        $(document).ready(resize);
        $(window).resize(resize);
        
        function resize(){
            var w = $(document).width();
            if(w>1200)
            {
            var t = w * 0.8 - document.getElementById('image-container').offsetWidth - 50;
            $('.desc').css({ width: t+'px' });
            $('.desc').show();
            }
            else
            {
                $('.desc').css({ width: '80%' });
                $('.desc').show();
            }
        }
    </script>
</html>
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
        <meta name="viewport" content="width=device-width" />
        
        <title>Flame - Folepuncher</title>
        
        <link rel="icon" type="image/png" href="../icon.png">
        
        <link rel="stylesheet" type="text/css" href="../styles/norm.css">
        
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
                <div id="image-container"><img src="folepuncher.png"/></div>
                <div class="desc">
                    <h1>Folepunchers</h1>
                    <p>The Folepuncher is so much more than just a folder. It is a piece of art. It's made out of solid, yet lightweight, plywood which means that it is durable making fraying folders and damaged documents problems of the past. Furthermore, it is personalisable. Any design you want can be precisely engraved onto it, from patterns to pictures to quotes. It is laser cut, meaning it is engraved more accurately than any human craftsman could ever achieve. Your name can also be engraved into the inside back cover and the file title can be cut into the side.</p>
                    <p>
However, the main feature of the Folepuncher is its hole punch inside. To us, as you need a hole puncher to use a folder and you need a folder to use a hole puncher, it seemed pointless to keep them as two separate objects. Instead, we have uniquely incorporated a hole puncher into the folder, meaning that they cannot be separated. The hole puncher cuts holes to perfectly match the ring binder. It is the first folder in the world to house a hole punch.
                    </p>
                    <p>
Almost all of the hole punchers that we saw were big and bulky, a contradiction to our goal of a sleek finish. Our team came up with a new design that would fit snugly next to the mechanism.
The finished product is one of ultimate beauty, giving an rush of excitement every time you open it. The look and feel is of exceptional craftsmanship. We are proud to be its creators, just like you should be the proud owner.
                    </p>
                    
                    <form method="post" action="" class="jcart">
                        <fieldset>
                            <input type="hidden" name="jcartToken" value="<?php echo $_SESSION['jcartToken'];?>" />
                            <input type="hidden" name="itm-id" value="1" />
                            <input type="hidden" name="itm-name" value="Folepuncher" />
                            <input type="hidden" name="itm-price" value="25.00" />
                            <input type="hidden" name="itm-url" value="folepuncher.php" />

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
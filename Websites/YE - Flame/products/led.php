<?php

include_once('jcart/jcart.php');
if (!isset($_SESSION)) {
  session_start();
}
?>

<!DOCTYPE html>
<html>
    
    <head>
        <meta http-equiv="content-type" content="text/html; charset=utf-8" />
        
        <title>Flame - Products</title>

        <!-- Bootstrap CSS -->
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css">
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap-theme.min.css">

        <!-- JCart stylesheet -->
        <link rel="stylesheet" type="text/css" media="screen, projection" href="../jcart/css/jcart.css" />
        
        <link rel="stylesheet" type="text/css" href="../styles/main.css">
        <link rel="stylesheet" type="text/css" href="../styles/products.css">
        <link rel="stylesheet" type="text/css" href="../styles/font.css">
        
        <link rel="icon" type="image/png" href="../icon.png">

    </head>
    
    <body>
        <header>
            <nav class="navbar navbar-inverse navbar-default" role="navigation">
              <div class="container-fluid">
                <!-- Brand and toggle get grouped for better mobile display -->
                <div class="navbar-header">
                  <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                  </button>
                  <span class="navbar-brand"><img class="logo" src="../logo.png"></span>
                </div>

                <!-- Collect the nav links, forms, and other content for toggling -->
                <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
                  <ul class="nav navbar-nav">
                    <li><a href="../index.html">Home</a></li>
                    <li id="prod-link" class="active">
                        <a href="#">Products<span class="caret"></span></a>
                        <div id="prod-bar">
                            <ul class="nav navbar-nav">
                        <li><a href="folepuncher.php">Folepunchers</a></li>
                        <li><a href="hand-warmer.php">Handwarmers</a></li>
                        <li><a href="#">LED Accessories</a></li>
                          <li><a href="upcycling.php">Bolders</a></li>
                      </ul>
                        </div>
                    </li>
                    <li><a href="../support.html">Support</a></li>
                    <li id="about-link">
                        <a href="#">About<span class="caret"></span></a>
                        <div id="about-bar">
                            <ul class="nav navbar-nav">
                        <li><a href="../trade-fairs.html">Trade Fairs</a></li>
                        <li><a href="../journey.html">Our Journey</a></li>
                        <li><a href="../ye.html">Young Enterprise</a></li>
                      </ul>
                        </div>  
                    </li>
                  </ul>
                </div>

                <div id="shop">
                    <a href="../checkout.php"><span class="icon-WheelEdition"></span></a>
                </div>

              </div>
            </nav>
        </header>

        <div id="containerMain">
            
            <div id="mainColumn">
                
                <div>
                
                    <div class="image-container"><img src="led.jpg"></div>

                    <div class="desc">
                        <h2>Led Accessories</h2>
                        <p>This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.This is a description.</p>

                    </div>
                    
                    <br class="clear">
                    
                </div>
                
                <div id="tabContainer">
                    
                    <div class="tabs">
                        <ul data-current="1">
                            <li id="tabHeader_1">Details</li>
                            <li id="tabHeader_2">Reviews</li>
                        </ul>
                    </div>
                    
                    <div class="tabscontent">
                        <div class="tabpage" id="tabpage_1">
                            <h4>Details:</h4>
                        </div>
                        
                        <div class="tabpage" id="tabpage_2">
                            <h4>Reviews:</h4>
                        </div>
                    </div>
                    
                </div>
                
            </div>
            
            <div id="adColumn"></div>
            
            <br class="clear">
            
        </div>
        
        <div class="slide-right">
                <div class="slide"><span class="icon-list"></span></div>
                
                <div class="slide-content">
                    <span class="icon-WheelEdition slide-icon"></span>
                    
                    <form method="post" action="" class="jcart">
                      <fieldset>
                        <input type="hidden" name="jcartToken" value="<?php echo $_SESSION['jcartToken'];?>" />
                        <input type="hidden" name="itm-id" value="1" />
                        <input type="hidden" name="itm-name" value="LED Accessories" />
                        <input type="hidden" name="itm-price" value="2.00" />
                        <input type="hidden" name="itm-url" value="led.php" />

                        <div class="slide-amount">
                            <label>Quantity:</label>
                            <br>
                            <input name="itm-qty" type="number" placeholder="Amount" onchange="updatePrice(this.value)">
                        </div>

                        <div class="slide-price">
                            <label>Price:</label>
                            <br>
                            <label class="price" id="price">£0.00</label>

                            <input name="add-button" type="submit" class="btn btn-lg btn-success" value="Add to cart">
                        </div>
                      </fieldset>
                    </form>
                </div>
        </div>
        
        <footer id="footer" class="navbar-inverse navbar-default">
            <p id="footer-content">
                2014-2015
            </p>

            <p id="footer-ye">
                A <a href="http://www.young-enterprise.org.uk/what-we-do/overview-new/">Young Enterprise</a> company
            </p>
        </footer>        
    </body>

    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>

    <!-- Latest compiled and minified JavaScript -->
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js"></script>
    
    <!-- JCart javascript -->
    <script type="text/javascript" src="jcart/js/jcart.min.js"></script>

    <script type="text/javascript">
        $(document).ready(function () {
            $('.dropdown').hover(function () {
                $(this).addClass('open');
            }, function () {
                $(this).removeClass('open');
            });
        });
        
        function updatePrice(val) {
            var price = document.getElementsByName("itm-price")[0].value;
            document.getElementById("price").innerHTML = "£"+(Math.max(0,parseInt(val)*price)).toFixed(2);
        }
        
        window.onload=function() {

          // get tab container
          var container = document.getElementById("tabContainer");
            // set current tab
            var navitem = container.querySelector(".tabs ul li");
            //store which tab we are on
            var ident = navitem.id.split("_")[1];
            navitem.parentNode.setAttribute("data-current",ident);
            //set current tab with class of activetabheader
            navitem.setAttribute("class","tabActiveHeader");

            //hide two tab contents we don't need
            var pages = container.querySelectorAll(".tabpage");
            for (var i = 1; i < pages.length; i++) {
              pages[i].style.display="none";
            }

            //this adds click event to tabs
            var tabs = container.querySelectorAll(".tabs ul li");
            for (var i = 0; i < tabs.length; i++) {
              tabs[i].onclick=displayPage;
            }
        }

        // on click of one of tabs
        function displayPage() {
          var current = this.parentNode.getAttribute("data-current");
          //remove class of activetabheader and hide old contents
          document.getElementById("tabHeader_" + current).removeAttribute("class");
          document.getElementById("tabpage_" + current).style.display="none";

          var ident = this.id.split("_")[1];
          //add class of activetabheader to new active tab and show contents
          this.setAttribute("class","tabActiveHeader");
          document.getElementById("tabpage_" + ident).style.display="block";
          this.parentNode.setAttribute("data-current",ident);
        }
    </script>

</html>
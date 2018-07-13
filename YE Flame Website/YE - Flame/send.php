<?php
$email = $_POST["email"];
$body = $_POST["body"];

require_once 'mandrill/Mandrill.php'; 

try {
    $mandrill = new Mandrill('93wElkdAdF5rs9Tpvft47A');
    $message = array(
        'html' => '<p>'.$email."<br>".$body.'</p>',
        'subject' => 'Support Request',
        'from_email' => 'auto@flame-ye.co.uk',
        'from_name' => 'Flame Auto',
        'to' => array(
            array(
                'email' => 'flameye.enquiries@gmail.com',
                'name' => 'Flame',
                'type' => 'to'
            )
        ),
        'headers' => array('Reply-To' => $email)
    );
    $async = false;
    $result = $mandrill->messages->send($message, $async);
    print_r($result);
} catch(Mandrill_Error $e) {
    echo 'A mandrill error occurred: ' . get_class($e) . ' - ' . $e->getMessage();
    throw $e;
}

header("Location: ./support.html");
exit;
?>
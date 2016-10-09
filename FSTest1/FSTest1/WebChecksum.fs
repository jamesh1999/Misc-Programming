module WebChecksum

open System.Net
open System.IO

type WebPage = WebPage of string

let getWebPage (url:string) = 
    let request = WebRequest.CreateHttp(url)
    let response = request.GetResponse()
    let sw = new StreamReader(response.GetResponseStream())
    sw.ReadToEnd()
    |> WebPage

module MD5 = 
    type Checksum = Checksum of System.Numerics.BigInteger

    let internal s = [7;12;17;22;7;12;17;22;7;12;17;22;7;12;17;22;5;9;14;20;5;9;
                            14;20;5;9;14;20;5;9;14;20;4;11;16;23;4;11;16;23;4;11;16;23;
                            4;11;16;23;6;10;15;21;6;10;15;21;6;10;15;21;6;10;15;21]

    let internal calculateRadians x =
        2.0 ** 32.0 * sin (float x)

    let internal k = 
        [0..63]
        |> List.map calculateRadians

    let internal takeBack n l =
        let util (acc: 'a list) (elem: 'a) =
            match acc.Length with
            | x when x < n -> elem::acc
            | _ -> acc

        List.foldBack util l

    let stringToBytes (str:string) =
        let rec f acc str =
            match str with
            | "" -> []
            | _ -> 
                //Prepend last character to accumulator
                str.Substring(0, str.Length - 1)
                |> f (str.[str.Length - 1]::acc)

        f [] str
        |> List.map int //Convert chars to ints

    let getChecksum (msg: int list) =
        let len = 8 * msg.Length
        let add = [(len >>> 24) &&& 0xff;(len >>> 16) &&& 0xff;(len >>> 8) &&& 0xff;len &&& 0xff]
        
        let numz' = (len + 1) % 512
        let numz = 
            match numz' with
            | x when x <= 448 -> 448 - numz'
            | x when x > 448 -> 960 - numz'

        let rec f rem l =
            match rem with
            | 0 -> l
            | _ -> 
                0::l
                |> f (rem - 1)

        let full = 
            f numz add
            |> List.append (1::msg)

        let rec hash a b c d msg =
            match msg with
            | [] -> (a,b,c,d)
            | _ ->
                let (x,y,z,w) = 
                    List.take 64 msg
                    |> hashUtil a b c d
                List.
                hash 
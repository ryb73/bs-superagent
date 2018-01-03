open Superagent;
open Js.Promise;
open Js.Result;

get "api/"
    |> Get.withCredentials
    |> Get.query @@ Js.Dict.fromList [("derp","hello"),("lmao","123")]
    |> Get.end_
    |> then_ @@ fun result => {
        switch result {
            | Error err => Js.log2 "Error:" err
            | Ok resp => {
                switch (Falsy.to_opt resp##error) {
                    | Some error => Js.log @@ "Error: " ^ error##message
                    | None => {
                        switch (Js.Nullable.to_opt resp##body) {
                            | None => Js.log "No Body"
                            | Some body => {
                                Js.log body;
                            }
                        }
                    }
                }
            }
        };

        resolve ();
    };

post "api/post"
    |> Post.withCredentials
    |> Post.send @@ Js.Json.object_ @@ Js.Dict.fromList [ ("var", Js.Json.string "var") ]
    |> Post.end_
    |> then_ @@ fun result => {
        Js.log result;
        resolve ();
    };
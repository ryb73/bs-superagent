open Superagent;
open Js.Promise;

get "api/"
    |> Get.withCredentials
    |> Get.query { "one" : 1, "two": "too" }
    |> Get.end_
    |> then_ @@ fun (err, res:option Superagent.result) => {
        resolve @@ switch (res) {
            | None => Js.log @@ "Error: " ^ (Js.String.make err)
            | Some res => {
                switch (Falsy.to_opt res##error) {
                    | Some error => Js.log @@ "Error: " ^ error##message
                    | None => {
                        switch (Js.Nullable.to_opt res##body) {
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
    |> Post.query { "query": 1 }
    |> Post.send { "var": "var" }
    |> Post.end_
    |> then_ @@ fun (err, res) => {
        Js.log err;
        Js.log res;
        resolve ();
    };
open Superagent;
open Js.Promise;

get "api/"
    |> Get.withCredentials
    |> Get.end_
    |> then_ @@ fun (err, res) => {
        resolve @@ switch (Js.Undefined.to_opt res) {
            | None => Js.log @@ "Error: " ^ (Js.String.make err)
            | Some res => {
                switch (Falsy.to_opt res##error) {
                    | Some error => Js.log @@ "Error: " ^ error##message
                    | None => {
                        switch (Js.Null.to_opt res##body) {
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
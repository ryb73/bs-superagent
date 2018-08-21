open Option.Infix;

module Make = (Promise : PromiseEx.Promise) => {
    open Promise;

    type request('a);
    type get;
    type post;

    [@decco]
    type reqError = {
        method: string,
        status: int,
        url: string,
        message: string,
        stack: string
    };

    [@decco]
    type result = {
        body: option(Js.Json.t),
        clientError: bool,
        info: bool,
        notFound: bool,
        ok: bool,
        serverError: bool,
        status: int,
        statusCode: int,
        statusText: option(string),
        statusType: int,
        text: string,
        error: [@decco.codec Decco.falseable] option(reqError)
    };

    type contentType =
        | ApplicationJson;

    type authorizationType =
        | Bearer;

    type header =
        | ContentType(contentType)
        | Accept(contentType)
        | Authorization(authorizationType, string);

    exception ReqError(result);
    exception ParseError(Js.Json.t, Decco.decodeError);

    [@bs.send] external withCredentials : request('a) => request('a) = "";
    [@bs.send.pipe : request('a)] external query : Js.Dict.t(string) => request('a) = "";

    [@bs.send] external _end :
        request('a)
        => (Js.nullable(string) => Js.nullable(Js.Json.t) => unit)
        => unit = "end";

    let end_ = (req) =>
        Promise.make((~resolve, ~reject as _) =>
            _end(req, (err, resp) => resolve(. (err, resp)))
        )
        |> then_(((err, resp)) =>
            switch (Js.Nullable.toOption(resp)) {
                | Some(resp) =>
                    switch ((Js.Nullable.toOption(err), result_decode(resp))) {
                        | (Some(errMsg), Error(_)) => Js.Exn.raiseError(errMsg)
                        | (Some(_), Ok(resp)) => raise(ReqError(resp))
                        | (None, Error(e)) => reject(ParseError(resp, e))
                        | (_, Ok(resp)) => resolve(resp)
                    }

                | None =>
                    Js.Nullable.toOption(err) |? "Unknown Error"
                        |> Js.Exn.raiseError
            }
        );

    [@bs.send.pipe : request('a)]
    external setHeaderCustom : string => string => request('a) = "set";

    let setHeader = (header, req) =>
        switch header {
            | ContentType(v) =>
                let key = "Content-Type";
                switch v {
                    | ApplicationJson => setHeaderCustom(key, "application/json", req)
                }

            | Accept(v) =>
                let key = "Accept";
                switch v {
                    | ApplicationJson => setHeaderCustom(key, "application/json", req)
                }

            | Authorization(authType, credentials) =>
                let key = "Authorization";
                switch authType {
                    | Bearer => setHeaderCustom(key, "Bearer " ++ credentials, req)
                }
        };

    [@bs.send.pipe: request(post)] external _send : string => request(post) = "send";
    let send = (json, req) => req
        |> setHeader(ContentType(ApplicationJson))
        |> _send(Js.Json.stringify(json));

    [@bs.module "superagent"] external get : string => request(get) = "";
    [@bs.module "superagent"] external post : string => request(post) = "";
};

include Make(Js.Promise);
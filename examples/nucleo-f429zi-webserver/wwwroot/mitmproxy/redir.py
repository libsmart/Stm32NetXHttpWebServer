from mitmproxy import http

def request(flow: http.HTTPFlow) -> None:
#     if flow.request.path.startswith("/api/Terminal/"):
#         flow.request.host = "localhost"
#         flow.request.port = 18888
#         return

    if flow.request.path.startswith("/api/Terminal/info"):
        flow.request.host = "localhost"
        flow.request.port = 18000
        return

#     if flow.request.path.startswith("/api/Terminal/person/terminateCostCenter"):
#         flow.request.host = "localhost"
#         flow.request.port = 18000
#         return

    if flow.request.path.startswith("/api/Terminal/person/comeAndGo"):
        flow.request.host = "localhost"
        flow.request.port = 18000
        return

#     if flow.request.path.startswith("/api/Terminal/person/info"):
#         flow.request.host = "localhost"
#         flow.request.port = 18000
#         return







```powershell

docker build -t testwww .

docker run -ti --rm --name TestWWW --volume ${PWD}/public:/var/www/html --volume ${PWD}/mitmproxy:/opt/mitmproxy -p 18080:18080 -p 18081:18081 -p 18000:18000 testwww


```


#include "head.h"
#include "Graph.h"
#include "MST.h"
#include "ui_Navigation.h"
#include <qDebug>
#include <sstream>
#include <QFile>

void Graph::Test(){
    uzi->label->setText("123");
}

Graph:: Graph(){
    tot = 0; nodeSz = 0; numOfEdge = 0;
    memset(head, 0, sizeof(head));
    V.push_back(Node1{string("ECNU"), true});   // The node of the storage location starts at 1
}

void Graph:: importFile(){
    string place1, place2, tmp;
    int dis;
    istringstream ss;
    QFile ifs("map.csv");
    if (ifs.open(QIODevice::ReadOnly | QIODevice::Text)){
        while (!ifs.atEnd()){
            QByteArray line = ifs.readLine();
            QString str(line);
            tmp = str.toStdString();
            ss.str(tmp);
            ss >> place1>> place2>> dis;
            uzi->listWidget->addItem(QString::fromStdString(place1)+' '+QString::fromStdString(place2)+' '+QString::number(dis));
            int u = find(V.begin(), V.end(), place1)-V.begin();
            if(u == (int)V.size()) V.push_back(Node1{place1, true}), nodeSz ++;
            int v = find(V.begin(), V.end(), place2)-V.begin();
            if(v == (int)V.size()) V.push_back(Node1{place2, true}), nodeSz ++;
            ins_edge(u, v, dis);
        }
        ifs.close();
    }
    uzi->listWidget->addItem("Initialize successfully!");
}

void Graph:: option(int op){ // select options and implement the transformation of input information
    string op1, op2, tmp;
    istringstream ss;
    int u, v, w, numOfPlaces, numOfConstraint;
    bool flag;
    string place1, place2;
    vector<int> placesID;
    unordered_map<int, vector<int> > constraints;
    switch (op){
    case 1:
        tmp = uzi->lineEdit->text().toStdString();
        ss.str(tmp);
        ss >> place1;
        u = find(V.begin(), V.end(), place1)-V.begin();
        if(u == (int)V.size()){
            V.push_back(Node1{place1, true});
            nodeSz ++;
            uzi->listWidget->addItem(QString::fromStdString(place1)+" insert place successully!");
        }else{
            if(!V[u].existed){
                V[u].existed = true;
                uzi->listWidget->addItem(QString::fromStdString(place1)+" insert place successully!");
            }else
                uzi->listWidget->addItem("The place has existed!");
        }
        break;
    case 2:
        tmp = uzi->lineEdit->text().toStdString();
        ss.str(tmp);
        ss>> place1>> place2>> w;
        u = find(V.begin(), V.end(), place1)-V.begin();
        if(u == (int)V.size()){ V.push_back(Node1{place1, true}); nodeSz ++;}
        v = find(V.begin(), V.end(), place2)-V.begin();
        if(v == (int)V.size()){ V.push_back(Node1{place2, true}); nodeSz ++;}
        ins_edge(u, v, w);
        break;
    case 3:
        place1 = uzi->lineEdit->text().toStdString();
        u = find(V.begin(), V.end(), place1)-V.begin();
        if(u != (int)V.size())
            del_place(u);
        else
            uzi->listWidget->addItem("The place does not exist!\n");
        break;
    case 4:
        /*while(cin>> op2){
            if(op2 != "place" && op2 != "road") 
                cout<< "Unknown action! Please select the desired delete action.\n1: place     2: road\n";
            else break;
        };*/
        tmp = uzi->lineEdit->text().toStdString();
        ss.str(tmp);
        ss>> place1>> place2;
        u = find(V.begin(), V.end(), place1)-V.begin();
        if(u == (int)V.size()) V.push_back(Node1{place1, false});
        v = find(V.begin(), V.end(), place2)-V.begin();
        if(v == (int)V.size()) V.push_back(Node1{place2, false});
        del_edge(u, v);
        break;
    case 5:
        tmp = uzi->lineEdit->text().toStdString();
        ss.str(tmp);
        ss>> place1>> place2;
        u = find(V.begin(), V.end(), place1)-V.begin();
        v = find(V.begin(), V.end(), place2)-V.begin();
        if(u == (int)V.size() || ! V[u].existed){
            uzi->listWidget->addItem(QString::fromStdString(place1)+" does not existed!");
            break;
        }else if(v == (int)V.size() || ! V[v].existed){
            uzi->listWidget->addItem(QString::fromStdString(place2)+" does not existed!");
            break;
        }
        getShortestPath(u, v);
        break;
    case 6:
        tmp = uzi->lineEdit->text().toStdString();
        ss.str(tmp);
        ss>> numOfPlaces;
        tmp = uzi->lineEdit_2->text().toStdString();
        ss.clear();
        ss.str(tmp);
        ss>> place1>> place2;
        u = find(V.begin(), V.end(), place1)-V.begin();
        v = find(V.begin(), V.end(), place2)-V.begin();
        if(u == (int)V.size() || ! V[u].existed){
            uzi->listWidget->addItem(QString::fromStdString(place1)+" does not exist!");
            break;
        }else if(v == (int)V.size() || ! V[v].existed){
            uzi->listWidget->addItem(QString::fromStdString(place2)+" does not exist!");
            break;
        }
        findPathThroNumPlaces(u, v, numOfPlaces);
        break;
    case 7:
        placesID.clear();
        flag = true;
        tmp = uzi->lineEdit->text().toStdString();
        ss.str(tmp);
        ss>> numOfPlaces;
        tmp = uzi->lineEdit_2->text().toStdString();
        ss.clear();
        ss.str(tmp);
        for(int i = 0; i < numOfPlaces; i ++){
            ss>> place1;
            u = find(V.begin(), V.end(), place1)-V.begin();
            if(u == (int)V.size()){
                flag = false;
                break;
            }
            placesID.push_back(u);
        }
        if(inConnectedGraph(placesID) && flag){
            buildMST(placesID);
        }else uzi->listWidget->addItem("There are places that cannot be reached (or do not exist)!");
        break;
    case 8:
        placesID.clear();
        flag = true;
        tmp = uzi->lineEdit->text().toStdString();
        ss.str(tmp);
        ss>> numOfPlaces;
        for(int i = 0; i < numOfPlaces; i ++){
            ss>> place1;
            u = find(V.begin(), V.end(), place1)-V.begin();
            if(u == (int)V.size()){
                flag = false;
                break;
            }
            placesID.push_back(u);
        }
        if(!(inConnectedGraph(placesID) && flag)){
            uzi->listWidget->addItem("There are places that cannot be reached (or do not exist)!");
            break;
        }
        tmp = uzi->lineEdit_2->text().toStdString();
        ss.clear();
        ss.str(tmp);
        ss>> numOfConstraint;
        flag = false;
        constraints.clear();
        for(int i = 0; i < numOfConstraint; i ++){
            ss>> place1>> place2;
            u = find(V.begin(), V.end(), place1)-V.begin();
            v = find(V.begin(), V.end(), place2)-V.begin();
            if(u == (int)V.size() || v == (int)V.size() || find(placesID.begin(), placesID.end(), u) == placesID.end() || find(placesID.begin(), placesID.end(), v) == placesID.end()){
                uzi->listWidget->addItem("There are places that do not exist (or do not in fixed places)!");
                break;
            }
            constraints[v].push_back(u);
        }
        findPathWithConstraint(placesID, constraints);
        break;
    default:
        cout<< "Unknown operation! Please enter the correct instructions!\n";
    }
    cout<< endl;
}
// task 3
void Graph:: ins_edge(int u, int v, int w){
    bool found = false;
    V[u].existed = true;
    V[v].existed = true;
    for(int i = head[u]; i; i = e1[i].nxt){
        int to = e1[i].to;
        if(to == v){
            if(e1[i].dis == inf){
                e1[i].dis = w;
                if(i&1) e1[i+1].dis = w;
                else e1[i-1].dis = w;
                uzi->listWidget->addItem("Insert successfully!");
            }else{
                uzi->listWidget->addItem("The edge has existed, modify successfully!");
                e1[i].dis = w;
                if(i&1) e1[i+1].dis = w;
                else e1[i-1].dis = w;
            }
            found = true;
            break;
        }    
    }
    if(!found){
        uzi->listWidget->addItem("Insert successfully!");
        e1[++ tot].nxt = head[u];
        e1[tot].to = v;
        e1[tot].dis = w;
        head[u] = tot;

        e1[++ tot].nxt = head[v];
        e1[tot].to = u;
        e1[tot].dis = w;
        head[v] = tot;
    }
}
// task 3
void Graph::del_edge(int u, int v){
    if(!head[u] || !head[v] || !V[u].existed || !V[v].existed) uzi->listWidget->addItem("The edge connecting "+QString::fromStdString(V[u].name)+" and "+QString::fromStdString(V[v].name)+" does not exist!");
    else{
        for(int i = head[u]; i ; i = e1[i].nxt){
            int to = e1[i].to;
            if(to == v){
                if(e1[i].dis == inf)
                    uzi->listWidget->addItem("The edge connecting "+QString::fromStdString(V[u].name)+" and "+QString::fromStdString(V[v].name)+" has been deleted!");
                else{
                    e1[i].dis = inf;
                    if(i&1) e1[i+1].dis = inf;
                    else e1[i-1].dis = inf;
                    uzi->listWidget->addItem("Delete successfully!");
                }
                break;
            }
        }
    }
}
// task 2
void Graph:: del_place(int u){
    V[u].existed = false;
    for(int i = head[u]; i; i = e1[i].nxt){
        if(e1[i].dis == inf) continue;
        else{
            e1[i].dis = inf;
            if(i&1) e1[i+1].dis = inf;
            else e1[i-1].dis = inf;
        }
    }
    uzi->listWidget->addItem("Delete place"+QString::fromStdString(V[u].name)+" successfully!");
}
// task 1
void Graph:: printEdges(int start){ // bfs to traverse all edge
    set<int> st1;
    queue<int> q;
    q.push(start); st1.insert(start);
    while(q.size()){
        int u = q.front(); q.pop();
        vis[u] = 1; 
        for (int i = head[u]; i; i = e1[i].nxt) {
            int v = e1[i].to;
            if(e1[i].dis == inf) continue;
            if (!vis[v]) {
                numOfEdge ++;
                uzi->listWidget->addItem(QString::fromStdString(V[u].name)+ ' '+QString::fromStdString(V[v].name)+' '+QString::number(e1[i].dis));
                if(!st1.count(v)) q.push(v), st1.insert(v);
            }
        }
    }

}
// task 4 5 7
vector<int> Graph:: getPath(int now){  // according to "pre" to get path
    vector<int> path;
    while(pre[now] != -1){
        path.push_back(now);
        now = pre[now];
    }
    path.push_back(now);
    return path;
}
// task 4
void Graph:: dijkstra(int s){ // the classical dijkstra algorithm to find single source shortest path
    priority_queue<Node2, vector<Node2>, greater<Node2>> q;
    q.push({0, s});
    while(!q.empty()){
        int u = q.top().v;
        q.pop();
        if(vis[u]) continue;
        vis[u] = 1;
        for(int i = head[u]; i; i = e1[i].nxt){
            int v = e1[i].to, w = e1[i].dis;
            if(dis[v] > dis[u]+w){
                dis[v] = dis[u]+w;
                pre[v] = u;
                q.push({dis[v], v});
            }
        }
    }
}
// task 4
void Graph:: getShortestPath(int u, int v){
    pre = new int[nodeSz+1];
    dis = new int[nodeSz+1];
    vis = new int[nodeSz+1];
    fill(vis, vis+nodeSz+1, 0);
    fill(dis, dis+nodeSz+1, inf);
    fill(pre, pre+nodeSz+1, 0);
    pre[u] = -1;
    dis[u] = 0;
    dijkstra(u);
    if(dis[v] == inf)
        uzi->listWidget->addItem("There is no path from "+QString::fromStdString(V[u].name)+" to "+QString::fromStdString(V[v].name)+"!");
    else{
        uzi->listWidget->addItem("[---Begin---] Outputs the shortest path");
        uzi->listWidget->addItem("The shortest path length is "+QString::number(dis[v]));
        vector<int> path = getPath(v);
        int sz = path.size();
        for(int i = sz-1; i > 0; i --){
            uzi->listWidget->addItem(QString::fromStdString(V[path[i]].name)+"->");
        }
        uzi->listWidget->addItem(QString::fromStdString(V[path[0]].name));
        uzi->listWidget->addItem("[---Finish---] Outputs the shortest path");
    }
    delete pre;
    delete dis;
    delete vis;
}
// task 1
void Graph:: print(){
    uzi->listWidget->addItem("[---Begin---] Outputs information about roads and locations");
    printPlaces();
    uzi->listWidget->addItem("Below is the current road information!");
    vis = new int[nodeSz+1];
    fill(vis, vis+nodeSz+1, 0);
    numOfEdge = 0;
    for(int i = 1; i <= nodeSz; i ++)   
        if(!vis[i])
            printEdges(i);
    uzi->listWidget->addItem("[---Finish---] Outputs information about roads and locations");
    uzi->label_3->setText("路径数："+QString:: number(numOfEdge));
    delete[] vis;
}
// task 5
void Graph:: dfsToFind(int tar, int now, int fa, int num, int numOfPlaces, int dis, int &ans, vector<int>& path){
    if(num == numOfPlaces){
        if(dis < ans && now == tar){ //if n locations are traversed to reach the tar and the distance is shorter
            ans = dis;
            path = getPath(now);
        }
        return;
    }
    vis[now] = 1;
    for(int i = head[now]; i; i = e1[i].nxt){
        int to = e1[i].to;
        if(vis[to]) continue;
        pre[to] = now;
        dfsToFind(tar, to, now, num+1, numOfPlaces, dis+e1[i].dis, ans, path);
    }
    vis[now] = 0;
}
// task 5
void Graph:: findPathThroNumPlaces(int u, int v, int num){ // find shortest path through n locations from u to v
    qDebug()<< "123";
    int ans = inf;
    vector<int> path;
    pre = new int[nodeSz+1];
    vis = new int[nodeSz+1];
    fill(vis, vis+nodeSz+1, 0);
    fill(pre, pre+nodeSz+1, 0);
    pre[u] = -1;
    dfsToFind(v, u, 0, 1, num, 0, ans, path); // dfs
    if(path.size()){
        uzi->listWidget->addItem("[---Begin---] Outputs the shortest path through n places");
        uzi->listWidget->addItem("The distance of shortest path through "+QString::number(num)+" places is "+QString::number(ans));
        uzi->listWidget->addItem("Below is the shortest path:");
        int sz = path.size();
        for(int i = sz-1; i > 0; i --){
            uzi->listWidget->addItem(QString::fromStdString(V[path[i]].name)+"->");
        }
        uzi->listWidget->addItem(QString::fromStdString(V[path[0]].name));
        uzi->listWidget->addItem("[---Finish---] Outputs the shortest path through n places");
    }else{
        uzi->listWidget->addItem("The shortest path through "+QString::number(num)+" places does not exist!");
    }
    delete pre;
    delete vis;
}
// task 1
void Graph:: printPlaces(){
    uzi->listWidget->addItem("Below is the current place information!");
    int existCitiesNum = 0;
    for(int i = 1; i <= nodeSz; i ++)
        if(V[i].existed){
            uzi->listWidget->addItem(QString::fromStdString(V[i].name)+ ' ');
            existCitiesNum ++;
        }
    if(!existCitiesNum) uzi->listWidget->addItem("No location information!");
    else uzi->listWidget->addItem("There are "+QString::number(existCitiesNum)+ " places!");
    uzi->label_2->setText("地点数："+QString:: number(existCitiesNum));
}
// task 6
void Graph:: dfs2(int u){ // simple dfs to determine connectivity
    vis[u] = 1;
    for(int i = head[u]; i; i = e1[i].nxt){
        int v = e1[i].to;
        if(vis[v] || e1[i].dis == inf) continue;
        dfs2(v);
    }
}
// task 6
bool Graph:: inConnectedGraph(vector<int>& placesID){ 
    if(!placesID.size()) return false;
    vis = new int[nodeSz+1];
    fill(vis, vis+nodeSz+1, 0);
    int root = placesID[0];
    dfs2(root);
    bool isConnected = true;
    for(auto i: placesID){
        if(!vis[i]){isConnected = false; break;}
    }
    delete vis;
    return isConnected;
}

// task 6
void Graph:: buildMST(vector<int>& places){
    MST T{tot};
    T.uzzi = uzi;
    queue<int> q;
    set<int> st1, st2;
    vector<int> path;
    q.push(places[0]); // put the required edge into MTS->e1
    while(!q.empty()){ // using bfs to traverse all edges
        int u = q.front(); q.pop(); 
        st1.insert(u);
        st2.insert(u);
        for(int i = head[u]; i; i = e1[i].nxt){
            int v = e1[i].to, w = e1[i].dis;
            if(w == inf) continue; 
            if(st1.find(v) == st1.end()){
                T.insert(u, V[u].name, v, V[v].name, w);
                if(st2.find(v) == st2.end()) q.push(v);
                st2.insert(v);
            }
        }
    }
    T.kruskal(); // build MST
    T.printMST(); // print MST
    T.floyd(); // calculate the shortest circuit of all sources
    places = T.findOptOrder(places); // find the shortest path and get the orders
    path = T.getPath(places); // according to visiting order to get path
    uzi->listWidget->addItem("[---Begin---] Outputs the shortest path on the minimum spanning tree");
    uzi->listWidget->addItem("On the minimum spanning tree, the shortest length is "+QString::number(T.minDis));
    uzi->listWidget->addItem("Here is the shortest path:\n"+QString::fromStdString(V[path[0]].name)+"->");
    for(int i = 1; i < (int)path.size(); i ++) uzi->listWidget->addItem(QString::fromStdString(V[path[i]].name));
    uzi->listWidget->addItem("[---Finish---] Outputs the shortest path on the minimum spanning tree");
}
// task 7
bool Graph:: isAllVisited(vector<int>& placesID){  // check whether all fixed places have been visited
    for(auto place: placesID)
        if(!vis[place]) return false;
    return true;
} 
// task 7
void Graph:: dfsWithConstraint(int u, int end, int dis, int &ans, vector<int>& path, vector<int>& placesID, unordered_map<int, vector<int> >& constraints){
    vis[u] = 1;
    if(u == end){
        if(isAllVisited(placesID) && dis < ans){ // if all places have been visited and the distance is shorter than best so far
            path = getPath(u);
            ans = dis;
        }
        vis[u] = 0;
        return;
    }
    for(int i = head[u]; i; i = e1[i].nxt){
        int to = e1[i].to;
        if(vis[to]) continue;
        bool preHaveVis = true;
        for(auto j: constraints[to]){  //check if all pre_places have been visited
            if(!vis[j]){
                preHaveVis = false;
                break;
            }
        }
        if(!preHaveVis) continue;
        pre[to] = u;
        dfsWithConstraint(to, end, dis+e1[i].dis, ans, path, placesID, constraints);
    }
    vis[u] = 0;
}

// task 7
void Graph:: findPathWithConstraint(vector<int>& placesID, unordered_map<int, vector<int> >& constraints){
// placesID: all places need to visit;   constraint: constraints on location access order
    int startPlace = placesID[0], endPlace = *placesID.rbegin();
    int ans = inf;
    pre = new int[nodeSz+1];
    vis = new int[nodeSz+1];
    fill(vis, vis+nodeSz+1, 0);
    fill(pre, pre+nodeSz+1, 0);
    pre[startPlace] = -1;
    vector<int> path;
    dfsWithConstraint(startPlace, endPlace, 0, ans, path, placesID, constraints);
    if(path.size()){
        uzi->listWidget->addItem("[---Begin---] Outputs the shortest path with constraint");
        uzi->listWidget->addItem("The distance of shortest path through all fixed places from is "+QString::number(ans));
        uzi->listWidget->addItem("Below is the shortest path(All the places we passed):");
        int sz = path.size();
        for(int i = sz-1; i > 0; i --){
            uzi->listWidget->addItem(QString::fromStdString(V[path[i]].name)+"->");
        }
        uzi->listWidget->addItem(QString::fromStdString(V[path[0]].name));
        uzi->listWidget->addItem("Below is the shortest path(All fixed places):");
        int tmp = 0;
        for(int i = sz-1; i >= 0; i --){
            if(find(placesID.begin(), placesID.end(), path[i]) != placesID.end()){
                if(tmp != (int)placesID.size()-1) uzi->listWidget->addItem(QString::fromStdString(V[path[i]].name)+"->"), tmp ++;
                else{uzi->listWidget->addItem(QString::fromStdString(V[path[i]].name)); break;}
            }
        }
        uzi->listWidget->addItem("[---Finish---] Outputs the shortest path with constraint");
    }else{
        uzi->listWidget->addItem("There is not legal way to visited all fixed places with constaint!");
    }

    delete vis;
    delete pre;
}











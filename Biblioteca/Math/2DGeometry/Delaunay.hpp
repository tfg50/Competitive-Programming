#ifndef DELAUNAY_H
#define DELAUNAY_H

namespace delaunay{
  struct edge{ int p[2], t[2]; };
  struct triangle{ int p[3], e[3]; };
  vector<edge> arestas;
  vector<triangle> ans;
  vector<point> pnts;
  vector<bool> apagado;
  ld INF = 4e5; //mais q 3x max coordenada
  int r3[] = {0,1,2,0,1,2,0,1,2};
  point circuncentro(point p, point q, point r){
    point a = p-r, b = q-r, c = point(a*(p+r)/2, b*(q+r)/2);
    return point(c%point(a.y,b.y), point(a.x,b.x)%c) /(a%b);
  }
  bool inside(point p, triangle t){
    bool ret=1;
    rep(i,0,3){
      if(between(pnts[t.p[i]], p, pnts[t.p[r3[i+1]]]))
        return 1;
      ret &= (ccw(pnts[t.p[i]], pnts[t.p[r3[i+1]]], p) == ccw(pnts[t.p[0]], pnts[t.p[1]], p));
    }
    return ret;
  }
  int criaedge(int pa, int pb, int ta, int tb=-1){
    edge novo;
    novo.p[0] = pa, novo.p[1] = pb, novo.t[0] = ta, novo.t[1] = tb;
    //cout << "Aresta nova: " << sz(arestas) << endl;
    //cout << "pontos:\n";
    //rep(i,0,2) cout << "(" << pnts[novo.p[i]].x << " " << pnts[novo.p[i]].y << " )\n";
    //cout << "tri:\n";
    //rep(i,0,2) cout << novo.t[i] << " ";
    //cout << endl << endl;
    arestas.pb(novo);
    return sz(arestas)-1;
  }
  void criatri(int pa, int pb, int pc, int ea, int eb, int ec){
    triangle novo;
    novo.p[0] = pa, novo.p[1] = pb, novo.p[2] = pc;
    novo.e[0] = ea, novo.e[1] = eb, novo.e[2] = ec;
    //cout << "CRIOU " << sz(ans) << endl;
    //cout << "pontos:\n";
    //rep(i,0,3) cout << "(" << pnts[novo.p[i]].x << " " << pnts[novo.p[i]].y << " )\n";
    //cout << "arestas:\n";
    //rep(i,0,3) cout << novo.e[i] << " ";
    //cout << endl << endl;
    ans.pb(novo);
    apagado.pb(0);
  }
  void apaga(int t){
    assert(t < sz(apagado));
    rep(i,0,3) rep(j,0,2) assert(arestas[ans[t].e[i]].t[j] != t);
    apagado[t]=1;
  }
  void testa_edge(int ed, int pa, int pb, int &e){
    if(arestas[ed].p[0] == pa && arestas[ed].p[1] == pb)
      e = ed;
    if(arestas[ed].p[0] == pb && arestas[ed].p[1] == pa)
      e = ed;
  }
  void flip(int a){
    if(arestas[a].t[0] == -1 || arestas[a].t[1] == -1) return;
    assert(max(arestas[a].p[0], arestas[a].p[1]) > 2);
    int ta = arestas[a].t[0], tb = arestas[a].t[1];
    int pa = arestas[a].p[0], pb=-1, pc = arestas[a].p[1], pd=-1;
    rep(i,0,3){
      if(ans[ta].p[i] != pa && ans[ta].p[i] != pc)
        pb = ans[ta].p[i];
      if(ans[tb].p[i] != pa && ans[tb].p[i] != pc)
        pd = ans[tb].p[i];  
    }
    point ca = circuncentro(pnts[pa], pnts[pb], pnts[pc]), cb = circuncentro(pnts[pa], pnts[pd], pnts[pc]);
    ld ra = dist(ca, pnts[pa]), rb = dist(cb, pnts[pa]);
    if(cmp(dist(ca,pnts[pd]), ra) >= 0 && cmp(dist(cb, pnts[pb]), rb) >= 0)
      return;
    //cout << "flipar aresta " << a << endl;
    int ea=-1, eb=-1, ec=-1, ed=-1;
    rep(i,0,3){
      testa_edge(ans[ta].e[i], pa, pb, ea);
      testa_edge(ans[ta].e[i], pb, pc, eb);
      testa_edge(ans[tb].e[i], pc, pd, ec);
      testa_edge(ans[tb].e[i], pd, pa, ed);
    }
    assert(ea != -1 && eb != -1 && ec != -1 && ed != -1);
    arestas[a].t[0] = arestas[a].t[1] = -1;
    int tc, td;
    int ex = criaedge(pd,pb,tc=sz(ans),td=sz(ans)+1);
    criatri(pd,pa,pb,ed,ea,ex);
    criatri(pd,pb,pc,ex,eb,ec);
    rep(i,0,2){
      if(arestas[ea].t[i] == ta)
        arestas[ea].t[i] = tc;
      if(arestas[eb].t[i] == ta)
        arestas[eb].t[i] = td;
      if(arestas[ed].t[i] == tb)
        arestas[ed].t[i] = tc;
      if(arestas[ec].t[i] == tb)
        arestas[ec].t[i] = td;
    }
    apaga(ta);
    apaga(tb);
    //cout << "flipou\n";
    flip(ea); flip(eb); flip(ec); flip(ed);
  }
  //void printa(int t){
  //  rep(i,0,3) cout << pnts[ans[t].p[i]].x << " " << pnts[ans[t].p[i]].y << " ";
  //  cout << endl;
  //}
  vector<vector<point> > triangulation(vector<point> vet){
    sort(all(vet));
    vet.resize(unique(all(vet))-vet.begin());
    random_shuffle(all(vet));
    pnts.resize(3); arestas.clear(); ans.clear(); apagado.clear();
    pnts[0] = point(-INF,-INF), pnts[1] = point(INF,-INF), pnts[2] = point(0,INF);
    criatri(0,1,2, criaedge(0,1,0), criaedge(1,2,0), criaedge(2,0,0));
    rep(i,0,sz(vet)){
      //cout << "quero: " << vet[i].x << " " << vet[i].y << endl;
      vi ids;
      int p = sz(pnts);
      pnts.pb(vet[i]);
      //rep(j,0,sz(ans)) if(!apagado[j] && inside(vet[i], ans[j]))
      //  ids.pb(j)/*, cout << "~~ " << j << endl*/;
      //assert(0 < sz(ids) && sz(ids) < 3);
       
      int cur = sz(ans)-1;
      while(1){
        int pcur = cur;
        assert(cur != -1);
        if(inside(vet[i], ans[cur])){
          ids.pb(cur);
          rep(j,0,3){
            int ar = ans[cur].e[j];
            if(between(pnts[arestas[ar].p[0]], vet[i], pnts[arestas[ar].p[1]])){
              rep(k,0,2) if(arestas[ar].t[k] != cur && arestas[ar].t[k] != -1)
                ids.pb(arestas[ar].t[k]);
            }
          }
          break;
        }
        rep(j,0,3){
          int pa = ans[cur].p[j], pb = ans[cur].p[r3[j+1]], pc = ans[cur].p[r3[j+2]];
          if(ccw(pnts[pa],pnts[pb],pnts[pc])*ccw(pnts[pa],pnts[pb],vet[i]) == -1){
            int ar = -1;
            rep(k,0,3)
              testa_edge(ans[cur].e[k], pa, pb, ar);
            assert(ar != -1);
            rep(k,0,2)
              if(arestas[ar].t[k] != pcur)
                cur = arestas[ar].t[k];
            break;
          }
        }
        assert(cur != pcur);
      }
       
      if(sz(ids)==1){
        int ta = ids[0];
        int tb = sz(ans), tc = sz(ans)+1, td = sz(ans)+2;
        int pa = ans[ta].p[0], pb = ans[ta].p[1], pc = ans[ta].p[2];
        int ea=-1, eb=-1, ec=-1;
        rep(j,0,3){
          testa_edge(ans[ta].e[j], pa, pb, ea);
          testa_edge(ans[ta].e[j], pb, pc, eb);
          testa_edge(ans[ta].e[j], pc, pa, ec);
        }
        assert(ea != -1 && eb != -1 && ec != -1);
        int ed = criaedge(p,pa,tb,tc);
        int ee = criaedge(p,pb,tc,td);
        int ef = criaedge(p,pc,td,tb);
        criatri(pa,p,pc,ec,ed,ef);
        criatri(p,pa,pb,ed,ea,ee);
        criatri(p,pb,pc,ee,eb,ef);
        rep(j,0,2){
          if(arestas[ec].t[j] == ta)
            arestas[ec].t[j] = tb;
          if(arestas[ea].t[j] == ta)
            arestas[ea].t[j] = tc;
          if(arestas[eb].t[j] == ta)
            arestas[eb].t[j] = td;
        }
        apaga(ta);
        flip(ea); flip(eb); flip(ec);
        flip(ed); flip(ee); flip(ef);
      }
      else{
        int ta = ids[0], tb = ids[1];
        assert(inside(vet[i], ans[tb]));
        int pa=-1, pb=-1, pc=-1, pd=-1;
        rep(j,0,3) if(between(pnts[ans[ta].p[j]], pnts[p], pnts[ans[ta].p[r3[j+1]]])){
          pb = ans[ta].p[j];
          pd = ans[ta].p[r3[j+1]];
          pa = ans[ta].p[r3[j+2]];
        }
        assert(pa!=-1);
        rep(j,0,3) if(between(pnts[ans[tb].p[j]], pnts[p], pnts[ans[tb].p[r3[j+1]]])){
          assert((pb == ans[tb].p[j] && pd == ans[tb].p[r3[j+1]]) || 
              (pb == ans[tb].p[r3[j+1]] && pd == ans[tb].p[j]));
          pc = ans[tb].p[r3[j+2]];
        }
        int ea=-1, eb=-1, ec=-1, ed=-1, ee=-1;
        rep(j,0,3){
          testa_edge(ans[ta].e[j], pa, pb, ea);
          testa_edge(ans[tb].e[j], pb, pc, eb);
          testa_edge(ans[tb].e[j], pc, pd, ec);
          testa_edge(ans[ta].e[j], pd, pa, ed);
          testa_edge(ans[ta].e[j], pd, pb, ee);
        }
        assert(ea != -1 && eb != -1 && ec != -1 && ed != -1 && ee != -1);
        int tc = sz(ans), td = sz(ans)+1, te = sz(ans)+2, tf = sz(ans)+3;
        int na = criaedge(pa,p,tf,tc);
        int nb = criaedge(pb,p,tc,td);
        int nc = criaedge(pc,p,td,te);
        int nd = criaedge(pd,p,te,tf);
        criatri(pa,pb,p,ea,na,nb);
        criatri(pb,pc,p,eb,nb,nc);
        criatri(pc,pd,p,ec,nc,nd);
        criatri(pd,pa,p,ed,nd,na);
        rep(j,0,2){
          if(arestas[ea].t[j] == ta)
            arestas[ea].t[j] = tc;
          if(arestas[eb].t[j] == tb)
            arestas[eb].t[j] = td;
          if(arestas[ec].t[j] == tb)
            arestas[ec].t[j] = te;
          if(arestas[ed].t[j] == ta)
            arestas[ed].t[j] = tf;
        }
        rep(j,0,2) arestas[ee].t[j] = -1;
        apaga(ta);
        apaga(tb);
        flip(ea); flip(eb); flip(ec); flip(ed);
        flip(na); flip(nb); flip(nc); flip(nd);
      }
      //cout << "botou " << i << " ( " << vet[i].x << " " << vet[i].y << " ) " << endl;
    }
    vector<vector<point> >ret;
    rep(i,0,sz(ans)) if(!apagado[i] && min(min(ans[i].p[0],ans[i].p[1]),ans[i].p[2]) > 2)
      ret.pb({pnts[ans[i].p[0]], pnts[ans[i].p[1]], pnts[ans[i].p[2]]});
    return ret;
  }
};

#endif
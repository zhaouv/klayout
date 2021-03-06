
/*

  KLayout Layout Viewer
  Copyright (C) 2006-2017 Matthias Koefferlein

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/



#include "dbEdge.h"
#include "utHead.h"


TEST(1) 
{
  db::Edge e (0, 0, 100, 200);
  db::Edge empty;

  EXPECT_EQ (empty.is_degenerate (), true);
  EXPECT_EQ (e.is_degenerate (), false);
  EXPECT_EQ (empty, db::Edge (0, 0, 0, 0));
  EXPECT_EQ (e, db::Edge (db::Point (0, 0), db::Point (100, 200)));
  EXPECT_EQ (e.p1 (), db::Point (0, 0));
  EXPECT_EQ (e.p2 (), db::Point (100, 200));
  EXPECT_EQ (e.dx (), 100);
  EXPECT_EQ (e.dy (), 200);
  EXPECT_EQ (e.transformed (db::Trans (1, db::Vector (0, 0))).dx (), -200);
  EXPECT_EQ (e.transformed (db::Trans (1, db::Vector (0, 0))).dx_abs (), db::Edge::distance_type (200));
  EXPECT_EQ (e.transformed (db::Trans (1, db::Vector (0, -100))).dy_abs (), db::Edge::distance_type (100));
  EXPECT_EQ (e.dy (), 200);
  EXPECT_EQ (e != db::Edge (db::Point (0, 0), db::Point (100, 200)), false);
  EXPECT_EQ (e == db::Edge (db::Point (0, 0), db::Point (100, 200)), true);
  EXPECT_EQ (e.moved (db::Vector (10, 20)), db::Edge (db::Point (10, 20), db::Point (110, 220)));
  EXPECT_EQ (e.enlarged (db::Vector (10, 20)), db::Edge (db::Point (-10, -20), db::Point (110, 220)));
  EXPECT_EQ (e.length (), db::coord_traits <db::Coord>::rounded_distance (sqrt (double (100*100+200*200))));
  EXPECT_EQ (e.sq_length (), 100*100+200*200);
  EXPECT_EQ (e.ortho_length (), size_t (100+200));
  EXPECT_EQ (e.to_string (), "(0,0;100,200)");
  EXPECT_EQ (e.swapped_points ().to_string (), "(100,200;0,0)");
  EXPECT_EQ (e.to_string (), "(0,0;100,200)");
  e.swap_points ();
  EXPECT_EQ (e.to_string (), "(100,200;0,0)");

  e.set_p1 (db::Point (1, 2));
  e.set_p2 (db::Point (11, 10));
  EXPECT_EQ (e.to_string (), "(1,2;11,10)");
  EXPECT_EQ (e.bbox ().to_string (), "(1,2;11,10)");

  EXPECT_EQ (e.extended (2).to_string (), "(-1,1;13,11)");
  EXPECT_EQ (e.extended (1).to_string (), "(0,1;12,11)");
  EXPECT_EQ (e.shifted (2).to_string (), "(0,4;10,12)");
  EXPECT_EQ (e.shifted (1).to_string (), "(0,3;10,11)");

  db::Edge ee;
  ee = e;
  ee.shift (2);
  EXPECT_EQ (ee.to_string (), "(0,4;10,12)");

  ee = e;
  ee.extend (2);
  EXPECT_EQ (ee.to_string (), "(-1,1;13,11)");

  EXPECT_EQ (db::Edge ().extended (2).to_string (), "(-2,0;2,0)");
  EXPECT_EQ (db::Edge ().shifted (2).to_string (), "(0,0;0,0)");
}

TEST(2) 
{
  db::Edge e (0, 0, 100, 200);

  EXPECT_EQ (e.parallel (db::Edge (10,20,110,220)), true);
  EXPECT_EQ (e.parallel (db::Edge (10,20,110,221)), false);
  EXPECT_EQ (e.parallel (db::Edge (10,20,110,219)), false);
  EXPECT_EQ (e.contains (db::Point (10,20)), true);
  EXPECT_EQ (e.contains (db::Point (100,200)), true);
  EXPECT_EQ (e.contains (db::Point (101,200)), false);
  EXPECT_EQ (e.contains (db::Point (50,100)), true);
  EXPECT_EQ (e.contains (db::Point (200,400)), false);
  EXPECT_EQ (e.contains (db::Point (-200,-400)), false);
  EXPECT_EQ (e.contains (db::Point (0,0)), true);
  EXPECT_EQ (db::Edge (10,20,110,230).distance (db::Point (100, 200)), -4);
  EXPECT_EQ (db::Edge (10,20,110,230).distance_abs (db::Point (100, 200)), db::Edge::distance_type (4));
  EXPECT_EQ (db::Edge (10,20,110,210).distance (db::Point (100, 200)), 4);
  EXPECT_EQ (db::Edge (10,20,110,222).distance (db::Point (100, 200)), -1);
  EXPECT_EQ (db::Edge (10,20,110,222).distance_abs (db::Point (100, 200)), db::Edge::distance_type (1));
  EXPECT_EQ (db::Edge (10,20,110,222).contains (db::Point (0, 0)), false);
  EXPECT_EQ (db::Edge (10,20,110,222).contains (db::Point (100, 200)), false);
}

TEST(3) 
{
  db::Edge e (0, 0, 100, 200);
  db::Edge empty;

  EXPECT_EQ (e.coincident (db::Edge (10,20,110,220)), true);
  EXPECT_EQ (e.coincident (db::Edge (10,20,110,222)), false);
  EXPECT_EQ (e.coincident (db::Edge (10,20,110,218)), false);
  EXPECT_EQ (e.coincident (db::Edge (110,220,220,440)), false);
  EXPECT_EQ (e.coincident (db::Edge (-110,-220,-220,-440)), false);
  EXPECT_EQ (e.coincident (db::Edge (100,200,100,400)), false);
  EXPECT_EQ (e.coincident (db::Edge (-100,-200,10,20)), true);
  EXPECT_EQ (e.coincident (db::Edge (-100,-200,0,0)), false);
}

TEST(4) 
{
  db::Edge e (0, 0, 100, 200);

  EXPECT_EQ (e.intersect (db::Edge (10,20,110,220)), true);
  EXPECT_EQ (e.intersect (db::Edge (10,-20,-110,220)), true);
  EXPECT_EQ (e.intersect (db::Edge (8,-20,-110,220)), false);
  EXPECT_EQ (e.intersect (db::Edge (20,0,-80,200)), true);
  EXPECT_EQ (e.intersect (db::Edge (20,10,-80,200)), true);
  EXPECT_EQ (e.intersect (db::Edge (10,20,-80,200)), true);
  EXPECT_EQ (e.intersect (db::Edge (8,20,-80,200)), false);
  EXPECT_EQ (e.intersect_point (db::Edge (10,20,110,220)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (10,-20,-110,220)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (8,-20,-110,220)).first, false);
  EXPECT_EQ (e.intersect_point (db::Edge (20,0,-80,200)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (20,10,-80,200)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (10,20,-80,200)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (8,20,-80,200)).first, false);
  EXPECT_EQ (e.intersect_point (db::Edge (10,20,110,220)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (10,20,110,220)).second, db::Point (10, 20));
  EXPECT_EQ (e.intersect_point (db::Edge (10,-20,-110,220)).second, db::Point (0, 0));
  EXPECT_EQ (e.intersect_point (db::Edge (20,0,-80,200)).second, db::Point (10, 20));
  EXPECT_EQ (e.intersect_point (db::Edge (20,10,-80,200)).second, db::Point (12, 25));
  EXPECT_EQ (e.intersect_point (db::Edge (10,20,-80,200)).second, db::Point (10, 20));
}

TEST(4a) 
{
  db::Edge e1 (db::Point (-134, 3629), db::Point (-130, 3649));
  db::Edge e2 (db::Point (-129, 3710), db::Point (-134, 3631));

  EXPECT_EQ (e1.cut_point (e2).second, db::Point (-134, 3628));
  EXPECT_EQ (e1.cut_point (e2).first, true);

  EXPECT_EQ (e1.intersect_point (e2).first, false);
}

TEST(4b) 
{
  db::Edge e1 (db::Point (-133, 3629), db::Point (-129, 3649));
  db::Edge e2 (db::Point (-129, 3710), db::Point (-134, 3631));

  EXPECT_EQ (e1.cut_point (e2).second, db::Point (-135, 3621));
  EXPECT_EQ (e1.cut_point (e2).first, true);

  EXPECT_EQ (e1.intersect_point (e2).first, false);
}

TEST(4c) 
{
  db::Edge e1 (db::Point (-135, 3629), db::Point (-129, 3649));
  db::Edge e2 (db::Point (-129, 3710), db::Point (-134, 3631));

  EXPECT_EQ (e1.cut_point (e2).second, db::Point (-134, 3633));
  EXPECT_EQ (e1.cut_point (e2).first, true);

  EXPECT_EQ (e1.intersect_point (e2).second, db::Point (-134, 3633));
  EXPECT_EQ (e1.intersect_point (e2).first, true);
}

TEST(4d) 
{
  db::Edge e1 (db::Point (-100, 1000), db::Point (100, 1000));
  db::Edge e2 (db::Point (101, 1000), db::Point (200, 1000));
  db::Edge e3 (db::Point (100, 1000), db::Point (200, 1000));
  db::Edge e4 (db::Point (50, 1000), db::Point (200, 1000));
  db::Edge e5 (db::Point (-150, 1000), db::Point (50, 1000));
  db::Edge e6 (db::Point (-150, 1000), db::Point (200, 1000));

  EXPECT_EQ (e1.intersect (e2), false);
  EXPECT_EQ (e1.intersect (e3), true);
  EXPECT_EQ (e1.intersect_point (e2).first, false);
  EXPECT_EQ (e1.intersect_point (e3).second, db::Point (100, 1000));
  EXPECT_EQ (e1.intersect_point (e3).first, true);
  EXPECT_EQ (e1.intersect_point (e4).second, db::Point (50, 1000));
  EXPECT_EQ (e1.intersect_point (e4).first, true);
  EXPECT_EQ (e1.intersect_point (e5).second, db::Point (-100, 1000));
  EXPECT_EQ (e1.intersect_point (e5).first, true);
  EXPECT_EQ (e1.intersect_point (e6).second, db::Point (-100, 1000));
  EXPECT_EQ (e1.intersect_point (e6).first, true);
}

TEST(5) 
{
  db::Edge e (0, 0, 1000000, 2000000);

  EXPECT_EQ (e.intersect (db::Edge (100000,200000,1100000,2200000)), true);
  EXPECT_EQ (e.intersect (db::Edge (100000,-200000,-1100000,2200000)), true);
  EXPECT_EQ (e.intersect (db::Edge (80000,-200000,-1100000,2200000)), false);
  EXPECT_EQ (e.intersect (db::Edge (200000,0,-800000,2000000)), true);
  EXPECT_EQ (e.intersect (db::Edge (200000,100000,-800000,2000000)), true);
  EXPECT_EQ (e.intersect (db::Edge (100000,200000,-800000,2000000)), true);
  EXPECT_EQ (e.intersect (db::Edge (80000,200000,-800000,2000000)), false);
  EXPECT_EQ (e.intersect_point (db::Edge (100000,200000,1100000,2200000)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (100000,-200000,-1100000,2200000)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (80000,-200000,-1100000,2200000)).first, false);
  EXPECT_EQ (e.intersect_point (db::Edge (200000,0,-800000,2000000)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (200000,100000,-800000,2000000)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (100000,200000,-800000,2000000)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (80000,200000,-800000,2000000)).first, false);
  EXPECT_EQ (e.intersect_point (db::Edge (100000,200000,1100000,2200000)).first, true);
  EXPECT_EQ (e.intersect_point (db::Edge (100000,200000,1100000,2200000)).second, db::Point (100000,200000));
  EXPECT_EQ (e.intersect_point (db::Edge (100000,-200000,-1100000,2200000)).second, db::Point (0, 0));
  EXPECT_EQ (e.intersect_point (db::Edge (200000,0,-800000,2000000)).second, db::Point (100000, 200000));
  EXPECT_EQ (e.intersect_point (db::Edge (200000,100000,-800000,2000000)).second, db::Point (123077,246154));
  EXPECT_EQ (e.intersect_point (db::Edge (100000,200000,-800000,2000000)).second, db::Point (100000, 200000));
}

TEST(6) 
{
  db::Edge e (0, 0, 1000, 2000);

  std::pair<bool, db::Point> ret;

  ret = e.projected (db::Point (-1000, 0));
  EXPECT_EQ (ret.first, false);

  ret = e.projected (db::Point (-1000, 500));
  EXPECT_EQ (ret.first, true);
  EXPECT_EQ (ret.second, db::Point (0, 0));

  ret = e.projected (db::Point (-1000, 700));
  EXPECT_EQ (ret.first, true);
  EXPECT_EQ (ret.second, db::Point (80, 160));
}

TEST(7) 
{
  db::Edge e1 (100, 200, 1000, 2000);
  db::Edge e2 (100, 200, 1000, 2000);
  db::Edge e3 (101, 200, 1001, 2000);
  db::Edge e4 (-200, 100, -2000, 1000);

  std::pair<bool, db::Point> ret;

  ret = e1.cut_point (e2);
  EXPECT_EQ (ret.first, false);

  ret = e1.cut_point (e3);
  EXPECT_EQ (ret.first, false);

  ret = e1.cut_point (e4);
  EXPECT_EQ (ret.first, true);
  EXPECT_EQ (ret.second, db::Point (0, 0));
}

TEST (8)
{
  db::DEdge e1 (-10.0, 0.0, 10.0, 0.0);
  db::DEdge e2 (-sqrt(2.0), -1.0, 2.0 - sqrt(2.0), 1.0);
  db::DEdge e3 (0.0, -100.0, 0.0, 10.0);
  db::DEdge e4 (-sqrt(2.0), 1.0, 2.0 - sqrt(2.0), -1.0);

  std::pair <bool, db::DPoint> i;
  db::Point ii;

  i = e1.intersect_point (e2);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e2.intersect_point (e1);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e3.intersect_point (e2);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e2.intersect_point (e3);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e1.intersect_point (e4);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e4.intersect_point (e1);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e3.intersect_point (e4);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e4.intersect_point (e3);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

}

TEST (9)
{
  db::DEdge e1 (10.0, 0.0, -10.0, 0.0);
  db::DEdge e2 (-sqrt(2.0), -1.0, 2.0 - sqrt(2.0), 1.0);
  db::DEdge e3 (0.0, 10.0, 0.0, -10.0);
  db::DEdge e4 (-sqrt(2.0), 1.0, 2.0 - sqrt(2.0), -1.0);

  std::pair <bool, db::DPoint> i;
  db::Point ii;

  i = e1.intersect_point (e2);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e2.intersect_point (e1);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e3.intersect_point (e2);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e2.intersect_point (e3);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e1.intersect_point (e4);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e4.intersect_point (e1);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e3.intersect_point (e4);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

  i = e4.intersect_point (e3);
  EXPECT_EQ (i.first, true);
  ii = db::Point (i.second);
  EXPECT_EQ (ii, db::Point (0, 0));

}

TEST(10)
{
  db::Edge b (db::Point (10, 20), db::Point (45, 60));
  db::Edge bc (db::Point (0, 1), db::Point (2, 3));

  std::string s (b.to_string ());
  EXPECT_EQ (s, "(10,20;45,60)");
  tl::Extractor ex (s.c_str ());
  ex.read (bc);

  EXPECT_EQ (bc.to_string (), s);
}

TEST(11)
{
  db::DEdge b (db::DPoint (10, 20), db::DPoint (45, 60));
  db::DEdge bc (db::DPoint (0, 1), db::DPoint (2, 3));

  std::string s (b.to_string ());
  EXPECT_EQ (s, "(10,20;45,60)");
  tl::Extractor ex (s.c_str ());
  ex.read (bc);

  EXPECT_EQ (bc.to_string (), s);
}

TEST(12)
{
  db::Edge a (db::DPoint (368, 726), db::DPoint (363, 734));
  db::Edge b (db::DPoint (353, 733), db::DPoint (375, 733));

  std::string s;
  s = a.intersect_point (b).second.to_string ();
  EXPECT_EQ (s, "364,733");
  s = b.intersect_point (a).second.to_string ();
  EXPECT_EQ (s, "364,733");
  a.swap_points ();
  s = a.intersect_point (b).second.to_string ();
  EXPECT_EQ (s, "364,733");
  s = b.intersect_point (a).second.to_string ();
  EXPECT_EQ (s, "364,733");
  a.swap_points ();
  b.swap_points ();
  s = a.intersect_point (b).second.to_string ();
  EXPECT_EQ (s, "364,733");
  s = b.intersect_point (a).second.to_string ();
  EXPECT_EQ (s, "364,733");
  a.swap_points ();
  s = a.intersect_point (b).second.to_string ();
  EXPECT_EQ (s, "364,733");
  s = b.intersect_point (a).second.to_string ();
  EXPECT_EQ (s, "364,733");
}

TEST(13)
{
  std::pair<bool, db::Edge> cl;
  cl = db::Edge (db::Point (0, 0), db::Point (100, 0)).clipped(db::Box (db::Point (50, -20), db::Point (60, 20)));
  EXPECT_EQ (cl.first, true);
  EXPECT_EQ (cl.second.to_string (), "(50,0;60,0)");
  cl = db::Edge (db::Point (0, 0), db::Point (100, 0)).clipped(db::Box (db::Point (50, 0), db::Point (60, 20)));
  EXPECT_EQ (cl.first, true);
  EXPECT_EQ (cl.second.to_string (), "(50,0;60,0)");
  cl = db::Edge (db::Point (0, 0), db::Point (100, 0)).clipped(db::Box (db::Point (50, 10), db::Point (60, 20)));
  EXPECT_EQ (cl.first, false);
  cl = db::Edge (db::Point (0, 0), db::Point (100, 0)).clipped(db::Box (db::Point (100, 0), db::Point (160, 20)));
  EXPECT_EQ (cl.first, true);
  EXPECT_EQ (cl.second.to_string (), "(100,0;100,0)");
  cl = db::Edge (db::Point (0, 0), db::Point (100, 0)).clipped(db::Box (db::Point (80, 0), db::Point (160, 20)));
  EXPECT_EQ (cl.first, true);
  EXPECT_EQ (cl.second.to_string (), "(80,0;100,0)");
  cl = db::Edge (db::Point (0, 0), db::Point (100, 0)).clipped(db::Box (db::Point (-100, 0), db::Point (0, 20)));
  EXPECT_EQ (cl.first, true);
  EXPECT_EQ (cl.second.to_string (), "(0,0;0,0)");
  cl = db::Edge (db::Point (0, 0), db::Point (100, 0)).clipped(db::Box (db::Point (-100, 0), db::Point (20, 20)));
  EXPECT_EQ (cl.first, true);
  EXPECT_EQ (cl.second.to_string (), "(0,0;20,0)");
  cl = db::Edge (db::Point (0, 0), db::Point (0, 0)).clipped(db::Box (db::Point (-100, 0), db::Point (20, 20)));
  EXPECT_EQ (cl.first, true);
  EXPECT_EQ (cl.second.to_string (), "(0,0;0,0)");
  cl = db::Edge (db::Point (0, 0), db::Point (0, 0)).clipped(db::Box (db::Point (-100, 0), db::Point (0, 20)));
  EXPECT_EQ (cl.first, true);
  EXPECT_EQ (cl.second.to_string (), "(0,0;0,0)");

  cl = db::Edge (db::Point (851, 98), db::Point (343, 466)).clipped(db::Box (db::Point (48, 134), db::Point (555, 438)));
  EXPECT_EQ (cl.first, true);
  EXPECT_EQ (cl.second.to_string (), "(555,312;382,438)");

  cl = db::Edge (db::Point (4, 0), db::Point (9, 2)).clipped(db::Box (db::Point (1, 2), db::Point (8, 6)));
  EXPECT_EQ (cl.first, true);
  //  Not nice but correct if you imagine that clipping "attracts" an edge:
  EXPECT_EQ (cl.second.to_string (), "(8,2;8,2)");

  //  It's important the both edges are connected:
  EXPECT_EQ (db::Edge (db::Point (0, 100), db::Point (600, 500)).clipped (db::Box (db::Point (100, 200), db::Point (200, 300))).second.to_string (), "(150,200;200,233)");
  EXPECT_EQ (db::Edge (db::Point (0, 100), db::Point (600, 500)).clipped (db::Box (db::Point (200, 200), db::Point (300, 300))).second.to_string (), "(200,233;300,300)");
}

TEST(14)
{
  db::Edge e = db::Edge (db::Point (0, 0), db::Point (100, 0));
  EXPECT_EQ (e.coincident (db::Edge (db::Point (0, 0), db::Point (100, 0))), true);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (100, 0))), true);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (200, 0))), true);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (0, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (1, 0))), true);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (100, 0), db::Point (200, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (99, 0), db::Point (200, 0))), true);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (99, 0), db::Point (99, 0))), false);
  e = db::Edge (db::Point (100, 0), db::Point (0, 0));
  EXPECT_EQ (e.coincident (db::Edge (db::Point (0, 0), db::Point (100, 0))), true);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (100, 0))), true);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (200, 0))), true);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (0, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (1, 0))), true);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (100, 0), db::Point (200, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (99, 0), db::Point (200, 0))), true);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (99, 0), db::Point (99, 0))), false);
  e = db::Edge (db::Point (100, 1), db::Point (0, 1));
  EXPECT_EQ (e.coincident (db::Edge (db::Point (0, 0), db::Point (100, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (100, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (200, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (0, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (1, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (100, 0), db::Point (200, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (99, 0), db::Point (200, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (99, 0), db::Point (99, 0))), false);
  e = db::Edge (db::Point (100, -1), db::Point (0, 1));
  EXPECT_EQ (e.coincident (db::Edge (db::Point (0, 0), db::Point (100, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (100, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (200, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (0, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (1, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (100, 0), db::Point (200, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (99, 0), db::Point (200, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (99, 0), db::Point (99, 0))), false);
  e = db::Edge (db::Point (50, 0), db::Point (50, 0));
  EXPECT_EQ (e.coincident (db::Edge (db::Point (0, 0), db::Point (100, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (100, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (200, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (0, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (-100, 0), db::Point (1, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (50, 0), db::Point (50, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (100, 0), db::Point (200, 0))), false);
  EXPECT_EQ (e.coincident (db::Edge (db::Point (49, 0), db::Point (200, 0))), false);
}


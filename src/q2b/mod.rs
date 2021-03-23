use crate::q2b::grid::Grid;
use crate::q2b::cluster_finder::ClusterFinder;

mod grid;
mod cluster_finder;


pub fn main() {
    let lx = 6;
    let ly = 4;
    let n = 10;
    let p_super = 0.1;
    let mut r: Vec<bool> = Vec::with_capacity(100);

    for _ in 0..100 {
        let g1 = Grid::new(lx, ly, n, p_super);
        let mut cf = ClusterFinder::new(&g1);
        cf.search();

        println!("{}", cf);
        r.push(cf.did_form_path());
    }

    let success = r.iter()
        .filter(|b| **b)
        .count();

    println!("Found path fraction = {}", (success as f64) / (r.len() as f64))
}

use crate::q2b::grid::Grid;
use std::collections::HashSet;
use rand::Rng;
use std::fmt;

type Pos = (usize, usize);

pub struct ClusterFinder<'a> {
    grid: &'a Grid,
    initial_point: Pos,

    // Cluster is a super set of next_to_process
    cluster: HashSet<Pos>,
    next_to_process: HashSet<Pos>,
}

// Cluster finder
impl <'a> ClusterFinder<'a> {
    pub fn new(grid: &Grid) -> ClusterFinder {
        let mut rng = rand::thread_rng();
        let init_x: usize = rng.gen_range(0usize..grid.grid.num_rows());
        let init_y: usize = rng.gen_range(0usize..grid.grid.num_columns());
        let initial_point = (init_x, init_y);

        ClusterFinder {
            grid,
            initial_point,

            cluster: hashset! {initial_point},
            next_to_process: hashset! {initial_point},
        }
    }

    pub fn search(&mut self) {
        while !self.next_to_process.is_empty() {
            self.step();
        }
    }

    fn step(&mut self) {
        let mut found: HashSet<Pos> = HashSet::new();

        // Iterate through each of next_to_process, removing them
        for pos in self.next_to_process.drain() {
            for reachable in self.grid.directly_reachable_from(pos) {
                // If not already in cluster, add to found as new point
                if !self.cluster.contains(&reachable) {
                    found.insert(reachable);
                }
            }
        }

        // Add these points into the cluster
        self.cluster.extend(found.iter());

        // All the new points we found are now in next_to_process
        self.next_to_process = found;
    }

    pub fn cluster(&self) -> &HashSet<Pos> {
        return &self.cluster;
    }

    pub fn did_form_path(&self) -> bool {
        let mut connected_top = false;
        let mut connected_bottom = false;

        // We dont need to find the path itself, just the **existence** of a path
        // This can be done by simply checking if our cluster contains points
        // which are connected to the top or bottom. If they exists, by their
        // construction, so does a path between them.
        for pos in &self.cluster {
            match pos {
                (_, 0) => connected_top = true,
                (_, t) if *t == self.grid.grid.num_rows() => connected_bottom = true,
                (_, _) => {}
            }

        }

        connected_bottom && connected_top
    }
}

impl <'a> fmt::Display for ClusterFinder<'a> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for x in 0..self.grid.grid.num_rows() {
            for y in 0..self.grid.grid.num_columns() {
                if self.initial_point == (x, y) {
                    write!(f, "0")?;
                } else if self.cluster.contains(&(x, y)) {
                    write!(f, "1")?;
                } else {
                    write!(f, ".")?;
                }
            }

            write!(f, "\n")?;
        }

        Ok(())
    }
}
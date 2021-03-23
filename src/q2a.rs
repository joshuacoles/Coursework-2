use array2d::Array2D;
use rand::Rng;
use rand::distributions::Uniform;

use std::ops::Range;
use std::collections::HashSet;
use std::fmt::Display;
use std::fmt;

struct Grid {
    grid: Array2D<GridCell>
}

#[derive(Copy, Clone, Eq, PartialEq, Debug)]
enum GridCell {
    Insulator,
    Conductor,
    SuperConductor,
}

impl fmt::Display for GridCell {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", match self {
            GridCell::Insulator => ".",
            GridCell::Conductor => "+",
            GridCell::SuperConductor => "x",
        })
    }
}

// To achieve uniform distribution of grids
fn random_indexes_in_range(range: Range<usize>, expected: usize) -> HashSet<usize> {
    let mut out = HashSet::with_capacity(expected);
    let uniform = Uniform::from(range);
    let mut rng = rand::thread_rng();

    while out.len() < expected {
        let i = rng.sample(uniform);
        out.insert(i);
    }

    return out;
}

impl Grid {
    fn new(lx: usize, ly: usize, n: usize, p_super: f64) -> Grid {
        let indexes = random_indexes_in_range(0..lx * ly, n);
        let mut counter: usize = 0;
        let mut rng = rand::thread_rng();

        let gen = || {
            counter += 1;
            let present = indexes.contains(&(counter - 1));

            if present {
                if rng.gen_bool(p_super) {
                    GridCell::SuperConductor
                } else {
                    GridCell::Conductor
                }
            } else {
                GridCell::Insulator
            }
        };

        Grid {
            grid: Array2D::filled_by_row_major(gen, ly, lx)
        }
    }

    fn directly_reachable_from(&self, pos: (usize, usize)) -> HashSet<Pos> {
        let (x, y) = pos;

        println!("{:?} = {:?}", pos, self.grid.get(pos.0, pos.1));

        let potential = match self.grid.get(pos.0, pos.1).expect("Ahh") {
            GridCell::Insulator => hashset![],

            GridCell::Conductor => hashset![
                (x.saturating_sub(1), y), (x.saturating_add(1), y),
                (x, y.saturating_sub(1)), (x, y.saturating_add(1)),
            ],

            GridCell::SuperConductor => hashset![
                (x.saturating_sub(1), y), (x.saturating_add(1), y),
                (x, y.saturating_sub(1)), (x, y.saturating_add(1)),
                (x.saturating_sub(1), y.saturating_sub(1)), (x.saturating_add(1), y.saturating_add(1)),
                (x.saturating_add(1), y.saturating_sub(1)), (x.saturating_sub(1), y.saturating_add(1)),
            ]
        };

        // Ensure all in valid range
        potential
            .iter()
            .filter(|(px, py)|
                (0..self.grid.num_rows()).contains(px) &&
                    (0..self.grid.num_columns()).contains(py)
            ).map(|p| p.clone())
            .collect()
    }
}

impl fmt::Display for Grid {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for row in self.grid.rows_iter() {
            for cell in row {
                write!(f, "{}", cell)?;
            }

            write!(f, "\n")?;
        }

        Ok(())
    }
}

type Pos = (usize, usize);

struct ClusterFinder {
    grid: Grid,
    initial_point: Pos,

    // In cluster super set of next_to_process
    in_cluster: HashSet<Pos>,
    next_to_process: HashSet<Pos>,
}

// Cluster finder
impl ClusterFinder {
    fn new(grid: Grid) -> ClusterFinder {
        let mut rng = rand::thread_rng();
        let init_x: usize = rng.gen_range(0usize..grid.grid.num_rows());
        let init_y: usize = rng.gen_range(0usize..grid.grid.num_columns());
        let initial_point = (init_x, init_y);

        ClusterFinder {
            grid,
            initial_point,

            in_cluster: hashset! {initial_point},
            next_to_process: hashset! {initial_point},
        }
    }

    fn search(&mut self) {
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
                if !self.in_cluster.contains(&reachable) {
                    found.insert(reachable);
                }
            }
        }

        // Add these points into the cluster
        self.in_cluster.extend(found.iter());

        // All the new points we found are now in next_to_process
        self.next_to_process = found;
    }
}

impl Display for ClusterFinder {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for x in 0..self.grid.grid.num_rows() {
            for y in 0..self.grid.grid.num_columns() {
                if self.initial_point == (x, y) {
                    write!(f, "0")?;
                } else if self.in_cluster.contains(&(x, y)) {
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

pub fn main() {
    let lx = 6;
    let ly = 4;
    let n = 10;

    for _ in 0..100 {
        let g1 = Grid::new(lx, ly, n, 0.1);
        let mut cf = ClusterFinder::new(g1);
        cf.search();
        println!("{}", cf);
    }
}

use array2d::Array2D;
use std::fmt;
use std::ops::Range;
use std::collections::HashSet;
use rand::{Rng, distributions::Uniform};

pub struct Grid {
    pub(crate) grid: Array2D<GridCell>
}

#[derive(Copy, Clone, Eq, PartialEq, Debug)]
pub enum GridCell {
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
    pub fn new(lx: usize, ly: usize, n: usize, p_super: f64) -> Grid {
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

    pub fn directly_reachable_from(&self, pos: (usize, usize)) -> HashSet<(usize, usize)> {
        let (x, y) = pos;

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

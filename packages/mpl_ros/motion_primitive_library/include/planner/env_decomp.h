/**
 * @file env_decomp.h
 * @biref environment for planning using decomposition
 */

#ifndef ENV_DECOMP_H
#define ENV_DECOMP_H
#include <planner/env_base.h>
#include <primitive/primitive.h>
#include <collision_checking/decomp_util.h>

namespace MPL {

/**
 * @brief Voxel map environment
 */
class env_decomp : public env_base
{
  protected:
    std::unique_ptr<DecompUtil> map_util_;
  public:

    ///Simple constructor
    env_decomp() {}
    ///Simple constructor
    env_decomp(const vec_Vec3f& obs, decimal_t r) {
      map_util_.reset(new DecompUtil(r));
      map_util_->setObstacles(obs);
    }

    ~env_decomp() {}

    ///Set goal state
    void set_goal(const Waypoint& goal) {
      goal_node_ = goal;
      goal_outside_ = false;
    }

    ///Check if a point is in free space
    bool is_free(const Vec3f& pt) const {
      return map_util_->isFree(pt);
    }

    /**
     * @brief Get successor
     * @param curr The node to expand
     * @param succ The array stores valid successors
     * @param succ_idx The array stores successors' Key
     * @param succ_cost The array stores cost along valid edges
     * @param action_idx The array stores corresponding idx of control for each successor
     * @param action_dts The array stores corresponding dt of control for each successor
     *
     * When goal is outside, extra step is needed for finding optimal trajectory
     * Here we use Heuristic function and multiply with 2
     */
    void get_succ( const Waypoint& curr, 
        std::vector<Waypoint>& succ,
        std::vector<Key>& succ_idx,
        std::vector<double>& succ_cost,
        std::vector<int>& action_idx,
        std::vector<double>& action_dts ) const
    {
      succ.clear();
      succ_idx.clear();
      succ_cost.clear();
      action_idx.clear();
      action_dts.clear();

      if(!map_util_->isFree(curr.pos)) {
        Ellipsoid new_ellipsoid;
        Polyhedron new_poly;
        if(!map_util_->addSeed(curr.pos, new_poly, new_ellipsoid))
          return;
        else {
          map_util_->addPoly(new_poly);
          map_util_->addEllipsoid(new_ellipsoid);
        }
      }

      ps_.push_back(curr.pos);

      for(int i = 0; i < (int) U_.size(); i++) {
        Primitive pr(curr, U_[i], dt_);
        Waypoint tn = pr.evaluate(dt_);
        if(pr.valid_vel(v_max_) && pr.valid_acc(a_max_)) {
          bool valid = true;
          decimal_t dt = dt_;
          if(!map_util_->isFree(pr, dt)) {
            valid = false;
            Waypoint tmp_tn = pr.evaluate(dt);
            Polyhedron new_poly;
            Ellipsoid new_ellipsoid;
            while(map_util_->addSeed(tmp_tn.pos, new_poly, new_ellipsoid)) {
              decimal_t new_dt = dt;
              if(!map_util_->intersectPolyhedron(pr, new_poly, dt, new_dt)) {
                valid = true;
                /*
                map_util_->addPoly(new_poly);
                map_util_->addEllipsoid(new_ellipsoid);
                for(const auto& it: new_polys)
                  map_util_->addPoly(it);
                  */
                break;
              }
              dt = new_dt;
              tmp_tn = pr.evaluate(dt);
            }
          }
          if(valid) {
            primitives_.push_back(pr);
            tn.use_pos = curr.use_pos;
            tn.use_vel = curr.use_vel;
            tn.use_acc = curr.use_acc;

            succ.push_back(tn);
            succ_idx.push_back(state_to_idx(tn));
            succ_cost.push_back(pr.J(wi_) + w_*dt_);
            action_idx.push_back(i);
            action_dts.push_back(dt_);
          }
        }
      }
    }

    Polyhedra polyhedra() {
      return map_util_->polyhedra();
    }
    vec_Ellipsoid ellipsoids() {
      return map_util_->ellipsoids();
    }


};
}


#endif

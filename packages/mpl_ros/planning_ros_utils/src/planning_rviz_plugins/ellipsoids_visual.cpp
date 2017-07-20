#include "ellipsoids_visual.h"

namespace planning_rviz_plugins {
  EllipsoidsVisual::EllipsoidsVisual(Ogre::SceneManager *scene_manager,
                                     Ogre::SceneNode *parent_node) {
    scene_manager_ = scene_manager;
    frame_node_ = parent_node->createChildSceneNode();
  }

  EllipsoidsVisual::~EllipsoidsVisual() {
    scene_manager_->destroySceneNode(frame_node_);
  }

  void EllipsoidsVisual::setMessage(const planning_ros_msgs::Ellipsoids::ConstPtr &msg) {
    objs_.clear();

    if (msg->ellipsoids.empty())
      return;

    objs_.resize(msg->ellipsoids.size());

    for (auto &it : objs_)
      it.reset(new rviz::Shape(rviz::Shape::Type::Sphere, scene_manager_,
                               frame_node_));

    int cnt = 0;
    for (const auto &it : msg->ellipsoids) {
      Mat3f E;
      for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
          E(i, j) = it.E[3 * i + j];
      Eigen::SelfAdjointEigenSolver<Mat3f> es(E);

      Ogre::Vector3 scale(2 * es.eigenvalues()[0], 2 * es.eigenvalues()[1],
                          2 * es.eigenvalues()[2]);
      objs_[cnt]->setScale(scale);

      Ogre::Vector3 d(it.d[0], it.d[1], it.d[2]);
      objs_[cnt]->setPosition(d);

      Quatf q(es.eigenvectors().determinant() * es.eigenvectors());
      Ogre::Quaternion o(q.w(), q.x(), q.y(), q.z());
      objs_[cnt]->setOrientation(o);
      cnt++;
    }
  }

  void EllipsoidsVisual::setFramePosition(const Ogre::Vector3 &position) {
    frame_node_->setPosition(position);
  }

  void EllipsoidsVisual::setFrameOrientation(
                                             const Ogre::Quaternion &orientation) {
    frame_node_->setOrientation(orientation);
  }

  void EllipsoidsVisual::setColor(float r, float g, float b, float a) {
    for (auto &it : objs_)
      it->setColor(r, g, b, a);
  }
}
